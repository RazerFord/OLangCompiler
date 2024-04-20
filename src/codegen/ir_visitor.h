#pragma once

#include <sstream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "visitor/visitor.h"

inline constexpr std::string ModuleName = "Main";
inline constexpr std::string CtorName = "<init>";
inline constexpr std::string PrefixNameMemberVar = "_var";
inline constexpr std::string PrefixVTable = "__VTable";

namespace ir_visitor {
class ir_visitor : public visitor::visitor {
 private:
  std::unique_ptr<llvm::LLVMContext> ctx_ =
      std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::IRBuilder<>> builder_ =
      std::make_unique<llvm::IRBuilder<>>(*ctx_);
  std::unique_ptr<llvm::Module> module_ =
      std::make_unique<llvm::Module>(ModuleName, *ctx_);

 public:
  void visit(details::program_node& p) override {
    register_types(p);
    register_members(p);
    register_vtables(p);

    module_->dump();
  }

 private:
  inline static std::string vtable_name(
      const std::shared_ptr<details::class_node>& c) {
    return PrefixVTable + c->get_class_name()->get_identifier()->get_name();
  }

  inline static std::vector<std::shared_ptr<details::member_node>> linelize(
      details::program_node& p, const std::shared_ptr<details::class_node>& c) {
    auto& tc = p.get_type_casting();
    std::string main_type = c->get_class_name()->get_identifier()->get_name();
    auto& ts1 = tc.at(main_type);
    std::vector<std::string> ts;
    for (const auto& [k, _] : ts1) {
      ts.push_back(k);
    }
    std::sort(ts.begin(), ts.end(),
              [&tc, &main_type](const std::string& l, const std::string& r) {
                return l == main_type ||
                       (tc.contains(l) && tc.at(l).contains(r));
              });
    std::vector<std::shared_ptr<details::member_node>> members;
    {
      std::set<std::string> added;
      const auto& scope = p.get_scope();
      for (const std::string& cls : ts) {
        auto cn = scope->find(cls);
        if (auto sp = std::dynamic_pointer_cast<details::class_node>(cn)) {
          for (const auto& m : sp->get_members()) {
            if (auto sp1 = std::dynamic_pointer_cast<details::method_node>(m)) {
              if (added.insert(sp1->mangle_method()).second) {
                members.push_back(m);
              }
            }
          }
        }
      }
    }
    return members;
  }

  void register_types(details::program_node& p) {
    for (const auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
      name = vtable_name(c);
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
    }
  }

  void register_members(details::program_node& p) {
    for (auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType* ptr_cls =
          llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name));
      c->set_class_type(ptr_cls);
      name = vtable_name(c);
      llvm::PointerType* ptrVTable =
          llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name))
              ->getPointerTo();

      std::vector<llvm::Type*> body{ptrVTable};
      // adding all types to a class
      {
        member_variable_visitor mvv(&body, this);
        for (const auto& m : c->get_members()) {
          m->visit(&mvv);
        }
      }

      // adding all methods to a class
      {
        member_method_visitor mmv(&body, this);
        for (const auto& m : c->get_members()) {
          m->visit(&mmv);
        }
      }
      ptr_cls->setBody(llvm::ArrayRef(body));
    }
  }

  void register_vtables(details::program_node& p) {
    for (const auto& c : p.get_classes()) {
      if (!c->get_extends()) continue;
      std::string vtable = vtable_name(c);
      auto ptr_table = llvm::StructType::getTypeByName(*ctx_, vtable);
      std::vector<llvm::Constant*> methods;
      std::vector<llvm::Type*> types;
      vtable_filler_visitor vfv(&methods, &types, this);

      std::vector<std::shared_ptr<details::member_node>> members =
          linelize(p, c);
      for (const auto& m : members) {
        m->visit(&vfv);
      }
      ptr_table->setBody(llvm::ArrayRef(types));
      module_->getOrInsertGlobal(vtable, ptr_table);
      auto global_table = module_->getNamedGlobal(vtable);
      global_table->setInitializer(
          llvm::ConstantStruct::get(ptr_table, llvm::ArrayRef(methods)));
    }
  }

  class member_variable_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Type*>* const body_ = nullptr;
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_variable_visitor(std::vector<llvm::Type*>* const body,
                                     const ir_visitor* ir_visitor)
        : body_{body}, ir_visitor_{ir_visitor} {}

    void visit(details::variable_node& v) override {
      std::string cls_name{
          v.get_type()->get_class_name()->get_identifier()->get_name()};
      llvm::Type* ptrCls = llvm::StructType::getTypeByName(
                               *ir_visitor_->ctx_, llvm::StringRef(cls_name));
      body_->push_back(ptrCls);
    }
  };

  class member_method_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Type*>* const methods_ = nullptr;
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_method_visitor(std::vector<llvm::Type*>* const methods,
                                   const ir_visitor* ir_visitor)
        : methods_{methods}, ir_visitor_{ir_visitor} {}

    void visit(details::method_node& f) override {
      std::vector<llvm::Type*> params;
      for (const auto& p : f.get_parameters()->get_parameters()) {
        std::string cls_name = p->get_type()->simple_type();
        llvm::Type* ptr_cls = llvm::StructType::getTypeByName(
            *ir_visitor_->ctx_, llvm::StringRef(cls_name));
        params.push_back(ptr_cls);
      }
      std::string ret_type_name = f.get_return_type()->simple_type();
      llvm::Type* ret_type = llvm::StructType::getTypeByName(
          *ir_visitor_->ctx_, llvm::StringRef(ret_type_name));

      if (!ret_type) {
        ret_type = llvm::Type::getVoidTy(*ir_visitor_->ctx_);
      }

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);
      f.set_method_type(ptr_func_type);

      llvm::Function* func = llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          f.get_identifier()->get_name(), *ir_visitor_->module_);
      f.set_method(func);

      methods_->push_back(ptr_func_type);
    }

    void visit(details::constructor_node& c) override {
      std::vector<llvm::Type*> params;
      for (const auto& p : c.get_parameters()->get_parameters()) {
        std::string cls_name = p->get_type()->simple_type();
        llvm::Type* ptr_cls = llvm::StructType::getTypeByName(
            *ir_visitor_->ctx_, llvm::StringRef(cls_name));
        params.push_back(ptr_cls);
      }
      const std::string* name = c.get_scope()->get_name(scope::scope_type::Class);
      auto ret_type = llvm::StructType::getTypeByName(*ir_visitor_->ctx_, llvm::StringRef(*name));

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);
      c.set_constr_type(ptr_func_type);

      llvm::Function::Create(ptr_func_type,
                             llvm::Function::LinkageTypes::ExternalLinkage,
                             CtorName, *ir_visitor_->module_);

      methods_->push_back(ptr_func_type);
    }
  };

  class vtable_filler_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Constant*>* const methods_ = nullptr;
    std::vector<llvm::Type*>* const types_ = nullptr;
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit vtable_filler_visitor(std::vector<llvm::Constant*>* const methods,
                                   std::vector<llvm::Type*>* const types,
                                   const ir_visitor* ir_visitor)
        : methods_{methods}, types_{types}, ir_visitor_{ir_visitor} {}

    void visit(details::method_node& f) override {
      methods_->push_back(f.get_method());
      types_->push_back(f.get_method()->getReturnType());
    }
  };

  class body_visitor : public visitor::visitor {
   public:
    void visit(details::expression_node& expression) override {}
  };
};
}  // namespace ir_visitor
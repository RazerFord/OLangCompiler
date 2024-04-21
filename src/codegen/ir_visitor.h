#pragma once

#include <set>
#include <sstream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
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
  std::unordered_map<std::string, llvm::Value*> var_env;

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

      llvm::Function* func = llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          f.get_identifier()->get_name(), *ir_visitor_->module_);

      f.set_method_value(func);
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
      const std::string* name =
          c.get_scope()->get_name(scope::scope_type::Class);
      auto ret_type = llvm::StructType::getTypeByName(*ir_visitor_->ctx_,
                                                      llvm::StringRef(*name));

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);

      auto func_value_ptr = llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          CtorName, *ir_visitor_->module_);

      c.set_constr_type(func_value_ptr);
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
      methods_->push_back(f.get_method_value());
      types_->push_back(f.get_method_value()->getReturnType());
    }
  };

  class func_def_visitor : public visitor::visitor {
    ir_visitor* ir_visitor_ = nullptr;

   public:
    func_def_visitor(ir_visitor* ir_visitor) : ir_visitor_(ir_visitor) {}

    void visit(details::method_node& method) override {
      auto func_value = method.get_method_value();
      llvm::BasicBlock* entry_basic_block =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "entry", func_value);
      ir_visitor_->builder_->SetInsertPoint(entry_basic_block);

      ir_visitor_->var_env.clear();
      for (auto& param : func_value->args()) {
        size_t paramNo = param.getArgNo();
        std::string param_name = method.get_parameters()
                                     ->get_parameters()[paramNo]
                                     ->get_identifier()
                                     ->get_name();
        llvm::Type* paramType =
            func_value->getFunctionType()->getParamType(paramNo);
        ir_visitor_->var_env[param_name] = ir_visitor_->builder_->CreateAlloca(
            paramType, nullptr, llvm::Twine(param_name));
        ir_visitor_->builder_->CreateStore(&param,
                                           ir_visitor_->var_env[param_name]);
      }

      // generate expr  and set return value
      method.get_body()->visit(this);
      llvm::verifyFunction(*func_value);
    }
    void visit(details::constructor_node& constr) override {
      auto func_value = constr.get_constr_value();
      llvm::BasicBlock* entry_basic_block =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "entry", func_value);
      ir_visitor_->builder_->SetInsertPoint(entry_basic_block);

      ir_visitor_->var_env.clear();
      for (auto& param : func_value->args()) {
        size_t paramNo = param.getArgNo();
        std::string param_name = constr.get_parameters()
                                     ->get_parameters()[paramNo]
                                     ->get_identifier()
                                     ->get_name();
        llvm::Type* paramType =
            func_value->getFunctionType()->getParamType(paramNo);
        ir_visitor_->var_env[param_name] = ir_visitor_->builder_->CreateAlloca(
            paramType, nullptr, llvm::Twine(param_name));
        ir_visitor_->builder_->CreateStore(&param,
                                           ir_visitor_->var_env[param_name]);
      }

      // generate expr  and set return value
      constr.get_body()->visit(this);
      llvm::verifyFunction(*func_value);
    }
  };

  class body_visitor : public visitor::visitor {
    ir_visitor* ir_visitor_ = nullptr;

   public:
    body_visitor(ir_visitor* ir_visitor) : ir_visitor_{ir_visitor} {}

    void visit(details::expression_node& expression) override {
      expression.get_final_object()->visit(this);
      expression.set_value(expression.get_final_object()->get_value());
    }
    void visit(details::body_node& body) override {}

    void visit(details::return_statement_node& return_expr) override {
      auto ret_type =
          return_expr.get_expression()->get_final_object()->get_value();
      if (ret_type->getType()->isVoidTy())
        ir_visitor_->builder_->CreateRetVoid();
      else
        ir_visitor_->builder_->CreateRet(ret_type);
    }

    void visit(details::variable_call& variable) override {

    }

    void visit(details::constructor_call& constr_call) override {
      if (constr_call.get_type()->simple_type() == "base") {
        //handle base call
        return;
      }
      auto obj = create_object(constr_call);

      auto callee_fun = constr_call.get_constructor()->get_constr_value();
      if (!callee_fun) {
        std::cout << "Function doesn't exist\n";
        return;
      }

      auto callee_fun_type = callee_fun->getFunctionType();
      std::vector<llvm::Value*> arg_values;
      auto args = constr_call.get_arguments();
      for (size_t i = 0; i < args.size(); i++) {
        args[i]->visit(this);
        llvm::Value* arg_val =
            std::dynamic_pointer_cast<details::expression_ext>(args[i])
                ->get_value();
        if (arg_val == nullptr) {
          std::cout << "Null Argument when calling function \n";
          return;
        }

        llvm::Type* paramTy = callee_fun_type->getParamType(i);
        llvm::Value* bitCastArgVal =
            ir_visitor_->builder_->CreateBitCast(arg_val, paramTy);
        arg_values.push_back(bitCastArgVal);
      }

      ir_visitor_->builder_->CreateCall(callee_fun, arg_values);

      constr_call.set_value(obj);
    }

    void visit(details::method_call& method_call) override {
      auto callee_fun = method_call.get_method()->get_method_value();
      if (!callee_fun) {
        std::cout << "Function doesn't exist\n";
        return;
      }

      auto callee_fun_type = callee_fun->getFunctionType();
      std::vector<llvm::Value*> arg_values;
      auto args = method_call.get_arguments();
      for (size_t i = 0; i < args.size(); i++) {
        args[i]->visit(this);
        llvm::Value* arg_val =
            std::dynamic_pointer_cast<details::expression_ext>(args[i])
                ->get_value();
        if (arg_val == nullptr) {
          std::cout << "Null Argument when calling function \n";
          return;
        }

        llvm::Type* paramTy = callee_fun_type->getParamType(i);
        llvm::Value* bitCastArgVal =
            ir_visitor_->builder_->CreateBitCast(arg_val, paramTy);
        arg_values.push_back(bitCastArgVal);
      }
      method_call.set_value(
          ir_visitor_->builder_->CreateCall(callee_fun, arg_values));
    }

   private:
    llvm::Value* create_object(details::constructor_call& constr_call) {
      return nullptr;
    }
  };
};
}  // namespace ir_visitor
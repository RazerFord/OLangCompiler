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
    for (const auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
      name = PrefixVTable + c->get_class_name()->get_identifier()->get_name();
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
    }
    for (const auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType* ptr_cls =
          llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name));
      name = PrefixVTable + c->get_class_name()->get_identifier()->get_name();
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
    std::string outstr;
    llvm::raw_string_ostream oss(outstr);

    module_->print(oss, nullptr, true, true);
    std::cout << outstr << std::endl;
  }

 private:
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
                               *ir_visitor_->ctx_, llvm::StringRef(cls_name))
                               ->getPointerTo();
      body_->push_back(ptrCls);
    }
  };

  class member_method_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Type*>* const methods_ = nullptr;
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_method_visitor(
        std::vector<llvm::Type*>* const methods,
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
      llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          f.get_identifier()->get_name(), *ir_visitor_->module_);

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
      auto ret_type = llvm::Type::getVoidTy(*ir_visitor_->ctx_);

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);
      llvm::Function::Create(ptr_func_type,
                             llvm::Function::LinkageTypes::ExternalLinkage,
                             CtorName, *ir_visitor_->module_);

      methods_->push_back(ptr_func_type);
    }
  };
};
}  // namespace ir_visitor
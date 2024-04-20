#pragma once

#include <sstream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "visitor/visitor.h"

inline constexpr std::string ModuleName = "Main";
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
      llvm::StructType* ptrCls = module_->getTypeByName(llvm::StringRef(name));
      name = PrefixVTable + c->get_class_name()->get_identifier()->get_name();
      llvm::PointerType* ptrVTable =
          module_->getTypeByName(llvm::StringRef(name))->getPointerTo();

      // adding all types to a class
      std::vector<llvm::Type*> body{ptrVTable};
      member_variable_visitor mvv(&body, this);
      for (const auto& m : c->get_members()) {
        m->visit(&mvv);
      }
      ptrCls->setBody(llvm::ArrayRef(body));

      // adding all methods to a class
      member_method_visitor mmv(this);
      for (const auto& m : c->get_members()) {
        m->visit(&mmv);
      }
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
      llvm::Type* ptrCls =
          ir_visitor_->module_->getTypeByName(llvm::StringRef(cls_name))
              ->getPointerTo();
      body_->push_back(ptrCls);
    }
  };

  class member_method_visitor : public visitor::visitor {
   private:
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_method_visitor(const ir_visitor* ir_visitor)
        : ir_visitor_{ir_visitor} {}

    void visit(details::method_node& f) override {

    }
  };
};
}  // namespace ir_visitor
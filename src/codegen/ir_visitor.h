#pragma once

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "visitor/visitor.h"

inline constexpr std::string ModuleName = "Main";
inline constexpr std::string PrefixVTable = "__VT";

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

  }
};
}  // namespace ir_visitor
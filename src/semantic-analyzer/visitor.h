#pragma once

#include <memory>
#include <unordered_set>

#include "./../logging/error_handler.h"
#include "scope-checking.h"
#include "syntactical-analyzer/details.h"

namespace visitor {
class visitor {
 public:
  virtual void visit(const details::identifier_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::primary_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::class_name_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::parameter_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::parameters_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::body_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::class_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::program_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::expression_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::variable_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::method_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::constructor_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::assignment_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::while_loop_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::if_statement_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::return_statement_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::arguments_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::this_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::null_node&,
                     error_handling::error_handling&) = 0;
  virtual void visit(const details::base_node&,
                     error_handling::error_handling&) = 0;

  virtual ~visitor() = default;
};

class scope_visitor : public visitor {
 private:
  std::shared_ptr<scope_checking::scope> scope_;

 public:
  void visit(const details::program_node& p,
             error_handling::error_handling& e) override {
    for (const auto& cls : p.get_classes()) {
      cls->visit(this);
    }
  }

  void visit(const details::class_node& cls,
             error_handling::error_handling& e) override {
    scope_ = scope_->push();
    for (const auto& m : cls.get_members()) {
      if (auto var = dynamic_cast<details::variable_node*>(m.get()); var) {
        var->visit(this);
      }
    }
    std::unordered_set<std::string> mangled_methods;
    for (const auto& m : cls.get_members()) {
      if (auto var = dynamic_cast<details::method_node*>(m.get()); var) {
        auto mangled_method = var->mangle_method();
        if (mangled_methods.contains(mangled_method)) {
          error_handling::error_t et{var->get_meta_info(),
                                     "error: class member cannot be redeclared"};
          e.register_error(et);
          continue;
        }
        mangled_methods.insert(mangled_method);
        var->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void visit(const details::variable_node& v,
             error_handling::error_handling& e) override {
    const std::string& key = v.get_identifier()->get_name();
    auto found = scope_->find(key);
    if (found) {
      error_handling::error_t et{v.get_meta_info(),
                                 "error: the class field is already declared"};
      e.register_error(et);
    } else {
      scope_->add(key, std::make_shared<details::variable_node>(v));
    }
  }

  void visit(const details::method_node& method,
             error_handling::error_handling& e) override {

  }
};
}  // namespace visitor
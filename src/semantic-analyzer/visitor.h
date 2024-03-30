#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "./../logging/error_handler.h"
#include "scope-checking.h"
#include "syntactical-analyzer/details.h"

namespace visitor {
inline const std::string VariableRedefinition =
    "a parameter with the same name is already defined";
inline error_handling::error_t make_error_t(const details::ast_node& node,
                                            const std::string& msg) {
  return error_handling::error_t{node.get_meta_info(), msg};
}

class visitor {
 public:
  virtual void visit(const details::identifier_node&){};
  virtual void visit(const details::primary_node&){};
  virtual void visit(const details::class_name_node&){};
  virtual void visit(const details::parameter_node&){};
  virtual void visit(const details::parameters_node&){};
  virtual void visit(const details::body_node&){};
  virtual void visit(const details::class_node&){};
  virtual void visit(const details::program_node&){};
  virtual void visit(const details::expression_node&){};
  virtual void visit(const details::variable_node&){};
  virtual void visit(const details::method_node&){};
  virtual void visit(const details::constructor_node&){};
  virtual void visit(const details::assignment_node&){};
  virtual void visit(const details::while_loop_node&){};
  virtual void visit(const details::if_statement_node&){};
  virtual void visit(const details::return_statement_node&){};
  virtual void visit(const details::arguments_node&){};
  virtual void visit(const details::this_node&){};
  virtual void visit(const details::null_node&){};
  virtual void visit(const details::base_node&){};
  virtual void visit(const details::literal_node<int32_t>&){};
  virtual void visit(const details::literal_node<bool>&){};
  virtual void visit(const details::literal_node<double_t>&){};

  virtual ~visitor() = default;
};

class scope_visitor : public visitor {
 private:
  std::shared_ptr<scope_checking::scope> scope_{new scope_checking::scope};
  error_handling::error_handling error_;

 public:
  void visit(const details::program_node& p) override {
    for (const auto& cls : p.get_classes()) {
      cls->visit(this);
    }
  }

  void visit(const details::class_node& cls) override {
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
          error_handling::error_t et{
              var->get_meta_info(), "error: class member cannot be redeclared"};
          error_.register_error(et);
          continue;
        }
        mangled_methods.insert(mangled_method);
        var->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void visit(const details::variable_node& v) override {
    const std::string& key = v.get_identifier()->get_name();
    auto found = scope_->find(key);
    if (found) {
      error_.register_error(
          make_error_t(v, "error: the class field is already declared"));
    } else {
      scope_->add(key, std::make_shared<details::variable_node>(v));
    }
  }

  void visit(const details::method_node& m) override {
    auto sym = std::make_shared<scope_checking::scope_symbol>();
    for (const auto& p : m.get_parameters()->get_parameters()) {
      std::string key = p->get_identifier()->get_name();
      if ((*sym)[key]) {
        error_.register_error(make_error_t(*p, VariableRedefinition));
        return;
      }
      (*sym)[key] = p;
    }
    scope_ = scope_->push(sym);
    for (const auto& s : m.get_body()->get_nodes()) {
      if (auto var = dynamic_cast<details::variable_node*>(s.get()); var) {
        std::string key = var->get_identifier()->get_name();
        if ((*sym)[key]) {
          error_.register_error(make_error_t(*s, VariableRedefinition));
          return;
        }
        (*sym)[key] = s;
      } else {
        s->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void visit(const details::expression_node& expr) override {
    expr.get_primary()->visit(this);
    for (const auto& e : expr.get_expression_values()) {
      if (e.second) {
        e.second->visit(this);
      }
    }
  }

  void visit(const details::class_name_node& c) override {
    std::string key = c.get_identifier()->get_name();
    if (auto var = scope_->find(key); !var) {
      error_.register_error(
          make_error_t(c, "variable \"" + key + "\" undefined"));
    }
  }

  void visit(const details::primary_node& p) override {
    throw std::runtime_error("unsupported operation");
  }

  void visit(const details::this_node& t) override {
    // this code block must be empty
  }

  void visit(const details::null_node& n) override {
    // this code block must be empty
  }

  void visit(const details::base_node& b) override {
    // this code block must be empty
  }

  void visit(const details::literal_node<int32_t>& l) override {
    // this code block must be empty
  }

  void visit(const details::literal_node<bool>&) override {
    // this code block must be empty
  }

  void visit(const details::literal_node<double_t>&) override {
    // this code block must be empty
  }

  void visit(const details::assignment_node& a) override {
    // this code block must be empty
  }

  void visit(const details::while_loop_node& w) override {
    w.get_expression()->visit(this);
    w.get_body_node()->visit(this);
  }

  void visit(const details::if_statement_node& i) override {
    i.get_expression()->visit(this);
    i.get_true_body()->visit(this);
    if (const auto& false_body = i.get_false_body(); false_body) {
      false_body->visit(this);
    }
  }

  void visit(const details::body_node& b) override {
    auto sym = std::make_shared<scope_checking::scope_symbol>();
    scope_ = scope_->push(sym);
    for (const auto& s : b.get_nodes()) {
      if (auto var = dynamic_cast<details::variable_node*>(s.get()); var) {
        std::string key = var->get_identifier()->get_name();
        if ((*sym)[key]) {
          error_.register_error(make_error_t(*s, VariableRedefinition));
          return;
        }
        (*sym)[key] = s;
      } else {
        s->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void print_error() const { error_.print_errors(); }
};
}  // namespace visitor
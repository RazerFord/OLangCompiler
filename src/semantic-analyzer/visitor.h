#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "./../logging/error_handler.h"
#include "scope.h"
#include "syntactical-analyzer/details.h"

namespace visitor {
inline const std::string body = "$body";
inline const std::string kw_this = "this";

inline std::string variable_redefinition(const std::string& name);
std::string variable_redefinition(const std::string& name) {
  return "variable named \"" + name + "\" is already defined";
}
inline error_handling::error_t make_error_t(const details::ast_node& node,
                                            const std::string& msg) {
  return error_handling::error_t{node.get_meta_info(), msg};
}

class visitor {
 public:
  virtual void visit(details::identifier_node&){};
  virtual void visit(details::primary_node&){};
  virtual void visit(details::type_node&){};
  virtual void visit(details::class_name_node&){};
  virtual void visit(details::parameter_node&){};
  virtual void visit(details::parameters_node&){};
  virtual void visit(details::body_node&){};
  virtual void visit(details::class_node&){};
  virtual void visit(details::program_node&){};
  virtual void visit(details::expression_node&){};
  virtual void visit(details::variable_node&){};
  virtual void visit(details::method_node&){};
  virtual void visit(details::constructor_node&){};
  virtual void visit(details::assignment_node&){};
  virtual void visit(details::while_loop_node&){};
  virtual void visit(details::if_statement_node&){};
  virtual void visit(details::return_statement_node&){};
  virtual void visit(details::arguments_node&){};
  virtual void visit(details::this_node&){};
  virtual void visit(details::null_node&){};
  virtual void visit(details::base_node&){};
  virtual void visit(details::literal_node<int32_t>&){};
  virtual void visit(details::literal_node<bool>&){};
  virtual void visit(details::literal_node<double_t>&){};
  virtual void visit(details::variable_call&){};
  virtual void visit(details::constructor_call&){};
  virtual void visit(details::method_call&){};
  virtual void visit(details::member_call&){};

  [[nodiscard]] virtual bool success() const noexcept { return false; };
  [[nodiscard]] virtual bool fail() const noexcept { return true; };

  virtual ~visitor() = default;
};

class scope_visitor : public visitor {
 private:
  std::shared_ptr<scope::scope> scope_{new scope::scope};
  error_handling::error_handling error_;
  bool success_;

  void register_error(const details::ast_node& node, const std::string& msg) {
    success_ = false;
    error_.register_error(make_error_t(node, msg));
  }

 public:
  void visit(details::program_node& p) override {
    success_ = true;
    p.set_scope(scope_);
    for (const auto& cls : p.get_classes()) {
      std::string name = cls->get_class_name()->get_identifier()->get_name();
      if (!scope_->find(name)) {
        scope_->add(name, cls);
      } else {
        register_error(*cls->get_class_name(),
                       "error: the " + name + " class is already defined");
      }
    }
    for (const auto& cls : p.get_classes()) {
      std::string class_name =
          cls->get_class_name()->get_identifier()->get_name();
      scope_ = scope_->push(class_name, scope::scope_type::Class);
      scope_->add(class_name, cls);
      scope_->add(kw_this, cls);
      cls->set_scope(scope_);
      cls->visit(this);
      scope_ = scope_->pop();
    }
  }

  void visit(details::class_node& cls) override {
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
          register_error(*var, "error: class member cannot be redeclared");
          continue;
        }
        mangled_methods.insert(mangled_method);
        var->visit(this);
      }

      if (auto var = dynamic_cast<details::constructor_node*>(m.get()); var) {
        auto mangled_ctr = var->mangle_ctr();
        if (mangled_methods.contains(mangled_ctr)) {
          register_error(*var, "error: constructor cannot be redeclared");
          continue;
        }
        mangled_methods.insert(mangled_ctr);
        var->visit(this);
      }
    }
  }

  void visit(details::variable_node& v) override {
    const std::string& key = v.get_identifier()->get_name();
    auto found = scope_->find(key);
    if (found) {
      register_error(v, "error: the field \"" + key +
                            "\" in this scope is already declared");
    } else {
      v.get_expression()->visit(this);
      scope_->add(key, v.shared_from_this());
      v.set_scope(scope_);
    }
  }

  void visit(details::method_node& m) override {
    auto sym = std::make_shared<scope::scope_symbol>();
    scope_ = scope_->push(sym, m.mangle_method(),
                          scope::scope_type::Method);  // change by pointer
    for (const auto& p : m.get_parameters()->get_parameters()) {
      std::string key = p->get_identifier()->get_name();
      if ((*sym)[key]) {
        register_error(*p, variable_redefinition(key));
      } else {
        (*sym)[key] = p;
        p->set_scope(scope_);
      }
    }
    for (const auto& s : m.get_body()->get_nodes()) {
      if (auto var = dynamic_cast<details::variable_node*>(s.get()); var) {
        std::string key = var->get_identifier()->get_name();
        if ((*sym)[key]) {
          register_error(*s, variable_redefinition(key));
        } else {
          var->get_expression()->visit(this);
          (*sym)[key] = s;
          var->set_scope(scope_);
        }
      } else {
        s->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void visit(details::constructor_node& ctr) override {
    auto sym = std::make_shared<scope::scope_symbol>();
    scope_ =
        scope_->push(sym, ctr.mangle_ctr(), scope::scope_type::Constructor);
    for (const auto& p : ctr.get_parameters()->get_parameters()) {
      std::string key = p->get_identifier()->get_name();
      if ((*sym)[key]) {
        register_error(*p, variable_redefinition(key));
      } else {
        (*sym)[key] = p;
        p->set_scope(scope_);
      }
    }
    for (const auto& s : ctr.get_body()->get_nodes()) {
      if (auto var = dynamic_cast<details::variable_node*>(s.get()); var) {
        std::string key = var->get_identifier()->get_name();
        if ((*sym)[key]) {
          register_error(*s, variable_redefinition(key));
        } else {
          (*sym)[key] = s;
          var->set_scope(scope_);
        }
      } else {
        s->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  void visit(details::expression_node& expr) override {
    expr.set_scope(scope_);
    expr.get_primary()->visit(this);
    for (const auto& e : expr.get_expression_values()) {
      if (e.second) {
        e.second->visit(this);
      }
    }
  }

  void visit(details::arguments_node& expr) override {
    for (const auto& arg : expr.get_arguments()) {
      arg->visit(this);
    }
  }

  void visit(details::type_node& type) override {
    type.get_class_name()->visit(this);
  }

  void visit(details::class_name_node& c) override {
    std::string key = c.get_identifier()->get_name();
    if (auto var = scope_->find(key); !var) {
      register_error(c, "error: token \"" + key + "\" undefined");
    } else {
      c.set_scope(scope_);
    }
  }

  void visit(details::return_statement_node& r) override {
    r.set_scope(scope_);
  }

  void visit(details::primary_node& p) override {
    throw std::runtime_error("unsupported operation");
  }

  void visit(details::assignment_node& a) override {
    const auto& left = a.get_lexpression();
    if (auto p =
            dynamic_cast<const details::this_node*>(left->get_primary().get());
        p != nullptr && left->get_expression_values().empty()) {
      register_error(*p, "\"this\" cannot be assigned");
    } else {
      a.get_lexpression()->visit(this);
      a.get_rexpression()->visit(this);
    }
  }

  void visit(details::while_loop_node& w) override {
    w.get_expression()->visit(this);
    w.get_body_node()->visit(this);
  }

  void visit(details::if_statement_node& i) override {
    i.get_expression()->visit(this);
    i.get_true_body()->visit(this);
    if (const auto& false_body = i.get_false_body(); false_body) {
      false_body->visit(this);
    }
  }

  void visit(details::body_node& b) override {
    auto sym = std::make_shared<scope::scope_symbol>();
    scope_ = scope_->push(sym, body, scope::scope_type::Body);
    for (const auto& s : b.get_nodes()) {
      if (auto var = dynamic_cast<details::variable_node*>(s.get()); var) {
        std::string key = var->get_identifier()->get_name();
        if ((*sym)[key]) {
          register_error(*s, variable_redefinition(key));
        } else {
          (*sym)[key] = s;
          var->set_scope(scope_);
        }
      } else {
        s->visit(this);
      }
    }
    scope_ = scope_->pop();
  }

  [[nodiscard]] bool success() const noexcept override { return success_; }

  [[nodiscard]] bool fail() const noexcept override { return !success_; }

  void print_error() const { error_.print_errors(); }
};

class type_visitor : public visitor {
 private:
  std::unordered_multimap<std::string, std::string> type_casting_ = {
      {"Integer", {"Real"}},
      {"Real", {"Integer"}},
      {"Integer", {"Any"}},
      {"Real", {"Any"}},
  };
  std::unordered_set<std::string> types_ = {"Integer", "Real", "Boolean"};
  std::unordered_map<std::string, std::string> constructor_calls_;
  error_handling::error_handling error_;
  bool success_;

  void register_error(const details::ast_node& node, const std::string& msg) {
    success_ = false;
    error_.register_error(make_error_t(node, msg));
  }

 public:
  void visit(details::program_node& p) override {
    success_ = true;
    for (const auto& cls : p.get_classes()) {
      std::string derived = cls->get_class_name()->get_identifier()->get_name();
      if (cls->get_extends()) {
        std::string base = cls->get_extends()->get_identifier()->get_name();
        type_casting_.insert({derived, base});
      }
      types_.insert(derived);
      type_casting_.insert({derived, "Any"});
    }
    std::cout << "//////////////////////////////////////////// CASTING "
                 "/////////////////////////////////////////////////\n";
    for (const auto& [k, v] : type_casting_) {
      std::cout << k << " -> " << v << '\n';
    }
    std::cout << "//////////////////////////////////////////// TYPES "
                 "/////////////////////////////////////////////////\n";
    for (const std::string& type : types_) {
      std::cout << type << '\n';
    }
    for (const auto& cls : p.get_classes()) {
      cls->visit(this);
    }
  }

  void visit(details::class_node& cls) override {
    for (const auto& m : cls.get_members()) {
      if (auto var = dynamic_cast<details::variable_node*>(m.get()); var) {
        var->visit(this);
      }
    }
    for (const auto& m : cls.get_members()) {
      if (auto method = dynamic_cast<details::method_node*>(m.get()); method) {
        method->visit(this);
      } else if (auto ctor = dynamic_cast<details::constructor_node*>(m.get());
                 ctor) {
        ctor->visit(this);
      }
    }
  }

  void visit(details::variable_node& v) override {
    if (!v.get_expression()->get_type()) {
      register_error(*v.get_expression(), "invalid expression");
    }
  }

  void visit(details::return_statement_node& r) override {
    if (auto method_name = r.get_scope()->get_name(scope::scope_type::Method)) {
      r.get_scope()->find(*method_name);
    }
  }

  void visit(details::method_node& m) override {
    m.get_parameters()->visit(this);
    // TODO: replace on void
    std::string return_type;
    if (m.get_return_type()) {
      return_type = m.get_return_type()->type();
    }
    body_checker bc(return_type);
    m.get_body()->visit(&bc);
  }

  void visit(details::constructor_node& ctr) override {
    ctr.get_parameters()->visit(this);
    // checking that the first expression in the constructor is "this"
    this_checker tc(ctr.mangle_ctr(), constructor_calls_);
    ctr.get_body()->visit(&tc);
    // TODO: replace on void
    body_checker bc("");
    ctr.get_body()->visit(&bc);
  }

  void visit(details::parameters_node& params) override {
    for (const auto& p : params.get_parameters()) {
      auto node = p->get_type()->get_class_name();
      const std::string& class_name = node->get_identifier()->get_name();
      auto type = p->get_scope()->find(class_name);
      if (!type) {
        register_error(*node, "\"" + class_name + "\" class not found");
      }
    }
  };

  void visit(details::expression_node& expr) override {}

  void visit(details::arguments_node& expr) override {}

  void visit(details::type_node& type) override {}

  void visit(details::class_name_node& c) override {}

  void visit(details::primary_node& p) override {
    throw std::runtime_error("unsupported operation");
  }

  void visit(details::assignment_node& a) override {}

  void visit(details::while_loop_node& w) override {}

  void visit(details::if_statement_node& i) override {}

  void visit(details::body_node& b) override {}

  [[nodiscard]] bool success() const noexcept override { return success_; }

  [[nodiscard]] bool fail() const noexcept override { return !success_; }

  void print_error() const { error_.print_errors(); }

 private:
  class this_checker : public visitor {
   private:
    const std::string scope_name_;
    const std::unordered_map<std::string, std::string>& constructor_calls_;
    bool is_this_ctor = false;
    bool is_base_ctor = false;

   public:
    explicit this_checker(
        std::string scope_name,
        std::unordered_map<std::string, std::string>& constructor_calls)
        : scope_name_{std::move(scope_name)},
          constructor_calls_{constructor_calls} {}

    void visit(details::body_node& b) override {
      const auto& nodes = b.get_nodes();
      if (!nodes.empty()) {
        nodes[0]->visit(this);
      }
    }

    void visit(details::expression_node& expr) override {
      expr.get_primary()->visit(this);
      if (is_this_ctor) {
        // TODO:
        // calculate mangling and
        // uncomment
        // std::string mangle_name = ...;
        // constructor_calls_(scope_name_, mangle_name);
        // std::cerr << "to add mangling" << std::endl;
      }
      if (is_base_ctor) {
        // TODO:
        // check the existence of a parent constructor
        // uncomment
        // std::string mangle_name = ...;
        // constructor_calls_(scope_name_, mangle_name);
        // std::cerr << "to add mangling" << std::endl;
      }
      is_this_ctor = false;
      is_base_ctor = false;
    }

    void visit(details::this_node& t) override { is_this_ctor = true; }
    void visit(details::base_node& b) override { is_base_ctor = true; }
  };

  class body_checker : public visitor {
   private:
    const std::string return_type_;

   public:
    explicit body_checker(std::string return_type)
        : return_type_{std::move(return_type)} {}

    void visit(details::body_node& b) override {
      for (auto& n : b.get_nodes()) {
        n->visit(this);
      }
    }

    void visit(details::expression_node& expr) override { expr.get_type(); }

    void visit(details::if_statement_node& i) override {
      if (i.get_expression()->get_type()->type() != "Boolean") {

      }
      i.get_true_body()->visit(this);
    }

    void visit(details::while_loop_node& w) override {}

    void visit(details::return_statement_node& r) override {}
  };
};

}  // namespace visitor
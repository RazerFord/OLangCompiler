#pragma once

#include <cmath>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "logging/error_handler.h"
#include "scope.h"
#include "syntactical-analyzer/details.h"
#include "visitor/visitor.h"

namespace visitor {
inline const std::string body = "$body";
inline const std::string kw_this = "this";

inline std::string variable_redefinition(const std::string& name);
std::string variable_redefinition(const std::string& name) {
  return "variable named \"" + name + "\" is already defined";
}

inline auto printf_class_node = std::make_shared<details::class_node>();

class semantic_visitor : public visitor {
 public:
  [[nodiscard]] virtual bool success() const noexcept { return false; };
  [[nodiscard]] virtual bool fail() const noexcept { return true; };

  ~semantic_visitor() override = default;
};

class generic_class_visitor : public semantic_visitor {
  std::string generic_name_;
  details::class_name_node& instaniated_class_name_;

 public:
  generic_class_visitor(const std::string& generic_name,
                        details::class_name_node& instaniated_class_name)
      : generic_name_(generic_name),
        instaniated_class_name_(instaniated_class_name) {}

  void visit(details::class_node& clazz) override {
    for (auto& member : clazz.get_members()) {
      member->visit(this);
    }
  }

  void visit(details::variable_node& var) override {
    var.get_expression()->visit(this);
  }

  void visit(details::method_node& method) override {
    for (auto& param : method.get_parameters()->get_parameters())
      param->visit(this);

    method.get_body()->visit(this);
    if (method.get_return_type()->get_class_name())
      method.get_return_type()->get_class_name()->visit(this);
  }

  void visit(details::constructor_node& constr) override {
    for (auto& param : constr.get_parameters()->get_parameters())
      param->visit(this);

    constr.get_body()->visit(this);
  }

  void visit(details::parameter_node& param) override {
    param.get_class_name()->get_class_name()->visit(this);
  }

  void visit(details::class_name_node& class_name) override {
    if (class_name.get_identifier()->get_name() == generic_name_)
      class_name = instaniated_class_name_;
  }

  void visit(details::body_node& bd_node) override {
    for (auto& expr : bd_node.get_nodes()) expr->visit(this);
  }

  void visit(details::return_statement_node& ret) override {
    ret.get_expression()->visit(this);
  }

  void visit(details::if_statement_node& if_node) override {
    if_node.get_expression()->visit(this);
    if_node.get_true_body()->visit(this);
    if_node.get_false_body()->visit(this);
  }

  void visit(details::while_loop_node& while_node) override {
    while_node.get_expression()->visit(this);
    while_node.get_body_node()->visit(this);
  }

  void visit(details::expression_node& expr) override {
    expr.get_primary()->visit(this);
    for (auto& value : expr.get_expression_values()) {
      if (value.second) value.second->visit(this);
    }
  }

  void visit(details::arguments_node& args) override {
    for (auto& arg : args.get_arguments()) arg->visit(this);
  }

  void visit(details::assignment_node& assign) override {
    assign.get_lexpression()->visit(this);
    assign.get_rexpression()->visit(this);
  }
};

class instantiate_visitor : public semantic_visitor {
  std::shared_ptr<scope::scope> scope_;

 public:
  instantiate_visitor(std::shared_ptr<scope::scope> scope) : scope_(scope) {}

  void visit(details::program_node& program) override {
    for (auto& clazz : program.get_classes()) {
      clazz->visit(this);
    }
  }

  void visit(details::class_node& clazz) override {
    for (auto& member : clazz.get_members()) {
      member->visit(this);
    }
  }

  void visit(details::variable_node& var) override {
    var.get_expression()->visit(this);
  }

  void visit(details::method_node& method) override {
    for (auto& param : method.get_parameters()->get_parameters())
      param->visit(this);

    method.get_body()->visit(this);
  }

  void visit(details::constructor_node& constr) override {
    for (auto& param : constr.get_parameters()->get_parameters())
      param->visit(this);

    constr.get_body()->visit(this);
  }

  void visit(details::parameter_node& param) override {
    param.get_class_name()->get_class_name()->visit(this);
  }

  void visit(details::class_name_node& class_name) override {
    if (scope_->find(class_name.get_full_name())) return;

    std::shared_ptr<details::class_node> generic_class;
    if (generic_class =
            scope_->find_generic_class(class_name.get_identifier()->get_name());
        !generic_class)
      return;

    auto cloned_generic_class =
        std::dynamic_pointer_cast<details::class_node>(generic_class->clone());
    cloned_generic_class->set_class_name(
        std::make_shared<details::class_name_node>(class_name));
    generic_class_visitor gen_visitor(
        generic_class->get_generic()->get_full_name(),
        *class_name.get_generic());
    cloned_generic_class->visit(&gen_visitor);

    auto program = std::dynamic_pointer_cast<details::program_node>(
        scope_->find("program"));
    program->add_class(cloned_generic_class);
    scope_->add(cloned_generic_class->get_class_name()->get_full_name(),
                cloned_generic_class);
  }

  void visit(details::body_node& bd_node) override {
    for (auto& expr : bd_node.get_nodes()) expr->visit(this);
  }

  void visit(details::return_statement_node& ret) override {
    ret.get_expression()->visit(this);
  }

  void visit(details::if_statement_node& if_node) override {
    if_node.get_expression()->visit(this);
    if_node.get_true_body()->visit(this);
    if (if_node.get_false_body()) {
      if_node.get_false_body()->visit(this);
    }
  }

  void visit(details::while_loop_node& while_node) override {
    while_node.get_expression()->visit(this);
    while_node.get_body_node()->visit(this);
  }

  void visit(details::expression_node& expr) override {
    expr.get_primary()->visit(this);
    for (auto& value : expr.get_expression_values()) {
      if (value.second) value.second->visit(this);
    }
  }

  void visit(details::arguments_node& args) override {
    for (auto& arg : args.get_arguments()) arg->visit(this);
  }

  void visit(details::assignment_node& assign) override {
    assign.get_lexpression()->visit(this);
    assign.get_rexpression()->visit(this);
  }
};

class scope_visitor : public semantic_visitor {
 private:
  std::unordered_map<std::string, std::shared_ptr<details::class_node>>
      class_node_by_name{};
  std::shared_ptr<scope::scope> scope_{new scope::scope};
  error_handling::error_handling error_;

  void register_error(const details::ast_node& node, const std::string& msg) {
    error_.register_error(error_handling::make_error_t(node, msg));
  }

 public:
  scope_visitor() = default;
  scope_visitor(std::shared_ptr<scope::scope> scope) : scope_(scope) {}

  void visit(details::program_node& p) override {
    for (const auto& cls : p.get_classes()) {
      class_node_by_name[cls->get_class_name()->get_full_name()] = cls;
    }
    p.set_scope(scope_);
    scope_->add("printf", printf_class_node);
    scope_->add("program", p.weak_from_this());

    for (const auto& cls : p.get_classes()) {
      std::string name = cls->get_class_name()->get_identifier()->get_name();

      if (!cls->is_generic_class() && !scope_->find(name)) {
        scope_->add(name, cls);
      } else if (cls->is_generic_class() && !scope_->find_generic_class(name)) {
        scope_->add_generic_class(name, cls);
      } else {
        register_error(*cls->get_class_name(),
                       "error: the " + name + " class is already defined");
      }
    }

    std::erase_if(p.get_classes(), [&scope = scope_](const auto& value) {
      return scope->find_generic_class(
          value->get_class_name()->get_identifier()->get_name());
    });

    instantiate_visitor inst_visitor(scope_);
    p.visit(&inst_visitor);

    for (const auto& cls : p.get_classes()) {
      std::string class_name = cls->get_class_name()->get_full_name();
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
    ctr.set_scope(scope_);
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
    std::string key = c.get_full_name();
    if (auto var = scope_->find(key); !var) {
      register_error(c, "error: token \"" + key + "\" undefined");
    } else {
      c.set_scope(scope_);
    }
  }

  void visit(details::return_statement_node& r) override {
    if (r.get_expression()) r.get_expression()->visit(this);
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
        var->get_expression()->visit(this);
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

  [[nodiscard]] bool success() const noexcept override {
    return !error_.error();
  }

  [[nodiscard]] bool fail() const noexcept override { return error_.error(); }

  void print_error() const { error_.print_errors(); }
};

namespace {
void transitive(
    std::unordered_map<std::string, std::unordered_map<std::string, bool>>& m) {
  for (auto& [k1, v1] : m) {
    for (auto& [k2, v2] : m) {
      for (auto& [k3, v3] : m) {
        auto it1 = m.find(k1);
        auto it2 = m.find(k2);
        if (it1 != m.end() && it2 != m.end() && it2->second.contains(k1) &&
            it1->second.contains(k3)) {
          m[k2][k3] = true;
        }
      }
    }
  }
}
}  // namespace

class type_visitor : public semantic_visitor {
 private:
  using type = details::type_node;

  std::unordered_map<std::string, std::unordered_map<std::string, bool>>
      type_casting_ = {
          {type::intT, {{type::realT, true}, {type::RealT, true}}},
          {type::realT, {{type::intT, true}, {type::IntegerT, true}}},
          {type::IntegerT,
           {{type::intT, true}, {type::RealT, true}, {type::AnyT, true}}},
          {type::RealT,
           {{type::realT, true}, {type::IntegerT, true}, {type::AnyT, true}}},
      };
  std::unordered_map<std::string, std::shared_ptr<details::class_node>>
      class_node_by_name{};
  std::unordered_set<std::string> types_ = {type::IntegerT, type::RealT,
                                            type::BooleanT, type::AnyT};
  std::unordered_map<std::string, std::string> constructor_calls_;
  error_handling::error_handling error_;

  void register_error(const details::ast_node& node, const std::string& msg) {
    error_.register_error(error_handling::make_error_t(node, msg));
  }

 public:
  void visit(details::program_node& p) override {
    for (const auto& cls : p.get_classes()) {
      class_node_by_name[cls->get_class_name()->get_full_name()] =
          cls;
    }

    for (const auto& cls : p.get_classes()) {
      std::string derived = cls->get_class_name()->get_full_name();
      if (cls->get_extends()) {
        std::string base = cls->get_extends()->get_full_name();
        type_casting_[derived][base] = true;
      }
      types_.insert(derived);
      type_casting_[derived][type::AnyT] = true;
      type_casting_[derived][derived] = true;
    }
    type_casting_["Super"]["Any"] = true;
    transitive(type_casting_);
    p.set_type_casting(type_casting_);

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
    if (!v.get_expression()->get_type(error_)) {
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
    std::string return_type = type::voidT;
    if (m.get_return_type()) {
      return_type = m.get_return_type()->simple_type();
    }
    body_checker bc(return_type, *this);
    m.get_body()->visit(&bc);
  }

  void visit(details::constructor_node& ctr) override {
    ctr.get_parameters()->visit(this);

    body_checker bc(type::voidT, *this);
    ctr.get_body()->visit(&bc);

    std::string type = *ctr.get_scope()->get_name(scope::scope_type::Class);
    auto class_node = class_node_by_name.at(type);
    if (class_node->get_extends()) {
      base_checker bac(ctr, *this);
      ctr.get_body()->visit(&bac);
    }
  }

  void visit(details::parameters_node& params) override {
    for (const auto& p : params.get_parameters()) {
      auto node = p->get_type()->get_class_name();
      const std::string& class_name = node->get_full_name();
      auto type = p->get_scope()->find(class_name);
      if (!type) {
        register_error(*node, "\"" + class_name + "\" class not found");
      }
    }
  }

  void visit(details::expression_node& expr) override {
    expr.get_object(error_);
  }

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

  [[nodiscard]] bool success() const noexcept override {
    return !error_.error();
  }

  [[nodiscard]] bool fail() const noexcept override { return error_.error(); }

  void print_error() const { error_.print_errors(); }

 private:
  class base_checker : public semantic_visitor {
   private:
    const details::constructor_node& ctor_;
    type_visitor& tv_;
    int i = 0;
    bool called = false;
    details::expression_node* last_expr{};

   public:
    base_checker(const details::constructor_node& ctor, type_visitor& tv)
        : ctor_{ctor}, tv_{tv} {}

    void visit(details::body_node& b) override {
      for (auto& n : b.get_nodes()) {
        n->visit(this);
        i++;
      }
      if (!called) {
        tv_.register_error(ctor_, "base must be called at least");
      }
    }

    void visit(details::constructor_call& expr) override {
      if ("base" == expr.get_type()->simple_type()) {
        if (i != 0) {
          tv_.register_error(*last_expr,
                             "base should be called on the first line");
        }
        called = true;
      }
    }

    void visit(details::expression_node& expr) override {
      last_expr = &expr;
      expr.get_object(tv_.error_)->visit(this);
    }
  };

  class body_checker : public semantic_visitor {
   private:
    const std::string ret_type_;
    type_visitor& tv_;

   public:
    explicit body_checker(std::string ret_type, type_visitor& tv)
        : ret_type_{std::move(ret_type)}, tv_{tv} {}

    void visit(details::body_node& b) override {
      for (auto& n : b.get_nodes()) {
        n->visit(this);
      }
    }

    void visit(details::expression_node& expr) override {
      expr.get_object(tv_.error_);
    }

    void visit(details::assignment_node& a) override {
      std::string ltype =
          a.get_lexpression()->get_type(tv_.error_)->simple_type();
      std::string rtype =
          a.get_rexpression()->get_type(tv_.error_)->simple_type();
      if (auto it = tv_.type_casting_.find(rtype);
          it != tv_.type_casting_.end() && !it->second.contains(ltype)) {
        tv_.register_error(
            *a.get_lexpression(),
            "error: casting error \"" + rtype + "\" to \"" + ltype + "\"");
      }
    }

    void visit(details::if_statement_node& i) override {
      std::string type_name =
          i.get_expression()->get_type(tv_.error_)->simple_type();
      if (type_name != type::booleanT && type_name != type::BooleanT) {
        tv_.register_error(
            *i.get_expression(),
            "error: in the \"if\" statement, bool type was expected");
      }
      i.get_true_body()->visit(this);
      if (auto& f = i.get_false_body()) {
        f->visit(this);
      }
    }

    void visit(details::while_loop_node& w) override {
      std::string type_name =
          w.get_expression()->get_type(tv_.error_)->simple_type();
      if (type_name != type::booleanT && type_name != type::BooleanT) {
        tv_.register_error(
            *w.get_expression(),
            "error: in the \"while\" statement, bool type was expected");
      }
      w.get_body_node()->visit(this);
    }

    void visit(details::return_statement_node& r) override {
      if (!r.get_expression() && ret_type_ != type::voidT) {
        tv_.register_error(r, "error: the expression \"" + ret_type_ +
                                  "\" was expected in the return");
      } else if (auto t =
                     r.get_expression()->get_type(tv_.error_)->simple_type();
                 t != ret_type_ && t != details::type_node::stdT) {
        tv_.register_error(*r.get_expression(),
                           "error: the type of expression \"" + t +
                               "\" does not match the return \"" + ret_type_ +
                               "\"");
      }
    }

    void visit(details::variable_node& v) override {
      if (!v.get_expression()->get_type(tv_.error_)) {
        tv_.error_.register_error(error_handling::make_error_t(
            *v.get_expression(), "invalid expression"));
      }
    }
  };
};

class indexer_visitor : public visitor {
  std::unordered_map<std::string, std::shared_ptr<details::class_node>>
      class_node_by_name{};

  void index_fields(details::class_node& cls) {
    int var_index = 0;
    std::stack<std::shared_ptr<details::class_node>> stack;
    std::string name = cls.get_class_name()->get_full_name();
    std::shared_ptr<details::class_node> c{class_node_by_name.at(name)};
    stack.push(c);
    while (c->get_extends()) {
      name = c->get_extends()->get_full_name();
      c = class_node_by_name.at(name);
      stack.push(c);
    }
    while (!stack.empty()) {
      c = stack.top();
      stack.pop();
      for (const auto& m : c->get_members()) {
        if (auto var = dynamic_cast<details::variable_node*>(m.get()); var) {
          var->set_index(var_index++);
        }
      }
    }
  }

  void index_methods(details::class_node& cls) {
    std::stack<std::shared_ptr<details::class_node>> stack;
    std::string name = cls.get_class_name()->get_full_name();
    std::shared_ptr<details::class_node> c{class_node_by_name.at(name)};
    stack.push(c);
    while (c->get_extends()) {
      name = c->get_extends()->get_full_name();
      c = class_node_by_name.at(name);
      stack.push(c);
    }
    int method_index = 0;
    std::unordered_map<std::string, int> mangled_methods;
    while (!stack.empty()) {
      c = stack.top();
      stack.pop();
      for (const auto& m : c->get_members()) {
        if (auto var = dynamic_cast<details::method_node*>(m.get()); var) {
          auto mangled_method = var->mangle_method();
          auto it = mangled_methods.find(mangled_method);
          if (it != mangled_methods.end()) {
            var->set_index(it->second);
          } else {
            mangled_methods[mangled_method] = method_index;
            var->set_index(method_index++);
          }
        }
      }
    }
  }

 public:
  void visit(details::program_node& p) override {
    for (const auto& cls : p.get_classes()) {
      class_node_by_name[cls->get_class_name()->get_full_name()] = cls;
    }
    for (const auto& cls : p.get_classes()) {
      cls->visit(this);
    }
  }

  void visit(details::class_node& cls) override {
    index_fields(cls);
    index_methods(cls);
  }
};
}  // namespace visitor

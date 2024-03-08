#pragma once

#include <memory>
#include <string>
#include <vector>

#include "lexical-analyzer/token.h"

namespace details {
enum class ast_token {
  Program,
  Class,
  ClassName,
  Member,
  Variable,
  Method,
  Constructor,
  Parameters,
  Parameter,
  Body,
  Statement,
  Assignment,
  WhileLoop,
  IfStatement,
  ReturnStatement,
  Expression,
  Primary,
  Identifier
};

using token::span;

struct meta {
  std::string name_;
  ast_token token_;
  token::span span_;

  explicit meta(const std::string& name, const ast_token& token,
                const span& span)
      : name_{name}, token_{token}, span_{span} {}

  explicit meta(const meta& meta)
      : name_{meta.name_}, token_{meta.token_}, span_{meta.span_} {}
};

class ast_node {
 protected:
  meta meta_info_;
  virtual bool validate() = 0;
  virtual void generate() = 0;

 public:
  virtual ~ast_node() {}
};

class class_node;
class class_name_node;
class parameters_node;
class parameter_node;
class body_node;
class statement_node;
class expression_node;
class primary_node;
class arguments_node;

class identifier_node : public ast_node {
 private:
  std::string name_;

 public:
  void set_name(const std::string& name) noexcept { name_ = name; }

  const std::string& get_name() const noexcept { return name_; }

  bool validate() override { return true; }

  void generate() override {}
};

class class_node : public ast_node {
  std::shared_ptr<class_name_node> class_name_;
  std::shared_ptr<class_name_node> extends_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = class_name;
  }

  void set_extends(std::shared_ptr<class_name_node> extends) {
    extends_ = extends;
  }
};

class class_name_node : public ast_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> generic_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = identifier;
  }
  void set_generic(std::shared_ptr<class_name_node> generic) {
    generic_ = generic;
  }
};

class program_node : public ast_node {
  std::vector<std::shared_ptr<class_node>> classes_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void add_class(std::shared_ptr<class_node> class_) {
    classes_.push_back(class_);
  }
};

class member_node : public ast_node {};

class variable_node : public member_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = identifier;
  }

  void set_expression(std::shared_ptr<expression_node> expression) {
    expression_ = expression;
  }
};

class method_node : public member_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<parameters_node> parameters_;
  std::shared_ptr<identifier_node> return_type_;
  std::shared_ptr<body_node> body_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = identifier;
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = parameters;
  }

  void set_body(std::shared_ptr<body_node> body) { body_ = body; }
  void set_return_type(std::shared_ptr<identifier_node> return_type) {
    return_type_ = return_type;
  }
};

class constructor_node : public member_node {
  std::shared_ptr<parameters_node> parameters_;
  std::shared_ptr<body_node> body_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = parameters;
  }

  void set_body(std::shared_ptr<body_node> body) { body_ = body; }
};

class parameters_node : public ast_node {
  std::vector<std::shared_ptr<parameter_node>> parameters_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void add_parameter(std::shared_ptr<parameter_node> parameter) {
    parameters_.push_back(parameter);
  }
};

class parameter_node : public ast_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> class_name_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = identifier;
  }
  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = class_name;
  }
};

class body_node : public ast_node {
 private:
  std::vector<std::shared_ptr<variable_node>> variables_;
  std::vector<std::shared_ptr<statement_node>> statements_;

 public:
  const std::vector<std::shared_ptr<variable_node>>& get_variables()
      const noexcept {
    return variables_;
  }

  const std::vector<std::shared_ptr<statement_node>>& get_statements()
      const noexcept {
    return statements_;
  }

  void set_variables(
      const std::vector<std::shared_ptr<variable_node>>& variables) noexcept {
    variables_ = variables;
  }

  void set_statements(
      const std::vector<std::shared_ptr<statement_node>>& statements) noexcept {
    statements_ = statements;
  }

  void add_variable(const std::shared_ptr<variable_node>& variable) noexcept {
    variables_.push_back(variable);
  }

  void add_statement(
      const std::shared_ptr<statement_node>& statement) noexcept {
    statements_.push_back(statement);
  }

  bool validate() override { return true; }

  void generate() override {}
};

class statement_node : public ast_node {};

class assignment_node : public statement_node {
 private:
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;

 public:
  const std::shared_ptr<identifier_node>& get_identifier() const noexcept {
    return identifier_;
  }

  const std::shared_ptr<expression_node>& get_expression() const noexcept {
    return expression_;
  }

  void set_identifier(
      const std::shared_ptr<identifier_node>& identifier) noexcept {
    identifier_ = identifier;
  }

  void set_expression(
      const std::shared_ptr<expression_node>& expression) noexcept {
    expression_ = expression;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class while_loop_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<body_node> body_;

 public:
  const std::shared_ptr<expression_node>& get_expression() const noexcept {
    return expression_;
  }

  const std::shared_ptr<body_node>& get_body_node() const noexcept {
    return body_;
  }

  void set_expression(
      const std::shared_ptr<expression_node>& expression) noexcept {
    expression_ = expression;
  }

  void set_body_node(const std::shared_ptr<body_node>& body_node) noexcept {
    body_ = body_node;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class if_statement_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<body_node> true_body_;
  std::shared_ptr<body_node> false_body_;

 public:
  const std::shared_ptr<expression_node>& get_expression() const noexcept {
    return expression_;
  }

  const std::shared_ptr<body_node>& get_true_body() const noexcept {
    return true_body_;
  }

  const std::shared_ptr<body_node>& get_false_body() const noexcept {
    return false_body_;
  }

  void get_expression(
      const std::shared_ptr<expression_node>& expression) noexcept {
    expression_ = expression;
  }

  void get_true_body(const std::shared_ptr<body_node>& true_body) noexcept {
    true_body_ = true_body;
  }

  void get_false_body(const std::shared_ptr<body_node>& false_body) noexcept {
    false_body_ = false_body;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class return_statement_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;

 public:
  const std::shared_ptr<expression_node>& get_expression() const noexcept {
    return expression_;
  }

  void get_expression(
      const std::shared_ptr<expression_node>& expression) noexcept {
    expression_ = expression;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class expression_node : public ast_node {
 private:
  std::shared_ptr<primary_node> primary_;
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<arguments_node> arguments_;

 public:
  const std::shared_ptr<primary_node>& get_primary() const noexcept {
    return primary_;
  }

  const std::shared_ptr<identifier_node>& get_identifier() const noexcept {
    return identifier_;
  }

  const std::shared_ptr<arguments_node>& get_arguments() const noexcept {
    return arguments_;
  }

  void set_primary(const std::shared_ptr<primary_node>& primary) noexcept {
    primary_ = primary;
  }

  void get_identifier(
      const std::shared_ptr<identifier_node>& identifier) noexcept {
    identifier_ = identifier;
  }

  void get_arguments(
      const std::shared_ptr<arguments_node>& arguments) noexcept {
    arguments_ = arguments;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class arguments_node : public ast_node {
 private:
  std::vector<std::shared_ptr<expression_node>> expressions_;

 public:
  const std::vector<std::shared_ptr<expression_node>>& get_expressions()
      const noexcept {
    return expressions_;
  }

  void set_expressions(const std::vector<std::shared_ptr<expression_node>>&
                           expression) noexcept {
    expressions_ = expression;
  }

  bool validate() override { return true; }

  void generate() override {}
};

class primary_node : public ast_node {
 private:
  std::string representation_;

 public:
  bool validate() override { return true; }

  void generate() override {}
};
}  // namespace details
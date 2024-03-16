#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "./../lexical-analyzer/token.h"

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
  token::span span_{};

  meta() = default;

  explicit meta(std::string name, const ast_token& token, const span& span)
      : name_{std::move(name)}, token_{token}, span_{span} {}

  meta(const meta& meta)
      : name_{meta.name_}, token_{meta.token_}, span_{meta.span_} {}
};

class ast_node {
 protected:
  meta meta_info_;
  virtual bool validate() = 0;
  virtual void generate() = 0;

 public:
  explicit ast_node() : meta_info_{meta{}} {}
  explicit ast_node(const meta& meta_info) : meta_info_{meta_info} {}

  virtual ~ast_node() = default;
};

class class_node;
class primary_node;
class class_name_node;
class parameters_node;
class parameter_node;
class member_node;
class method_node;
class statement_node;
class expression_node;
class arguments_node;
class body_node;
class this_node;
class null_node;

class identifier_node : public ast_node {
 private:
  std::string name_;

 public:
  void set_name(const std::string& name) noexcept { name_ = name; }

  [[nodiscard]] const std::string& get_name() const noexcept { return name_; }

  bool validate() override { return true; }

  void generate() override {}
};

class class_node : public ast_node {
  std::shared_ptr<class_name_node> class_name_;
  std::shared_ptr<class_name_node> extends_;
  std::vector<std::shared_ptr<member_node>> members_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<class_name_node>& get_class_name() const {
    return class_name_;
  }

  [[nodiscard]] const std::shared_ptr<class_name_node>& get_extends() const {
    return extends_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<member_node>>& get_members()
      const {
    return members_;
  }

  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = std::move(class_name);
  }

  void set_extends(std::shared_ptr<class_name_node> extends) {
    extends_ = std::move(extends);
  }

  void add_member(std::shared_ptr<member_node> member) {
    members_.push_back(std::move(member));
  }
};

class primary_node : public ast_node {
 private:
  std::string representation_;
};

class class_name_node : public primary_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> generic_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<class_name_node>& get_generic() const {
    return generic_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
  }

  void set_generic(std::shared_ptr<class_name_node> generic) {
    generic_ = std::move(generic);
  }
};

class program_node : public ast_node {
  std::vector<std::shared_ptr<class_node>> classes_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<class_node>>& get_classes()
      const {
    return classes_;
  }

  void add_class(std::shared_ptr<class_node> class_) {
    classes_.push_back(std::move(class_));
  }
};

class member_node : public ast_node {};

class variable_node : public member_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression() const {
    return expression_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
  }

  void set_expression(std::shared_ptr<expression_node> expression) {
    expression_ = std::move(expression);
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
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<parameters_node>& get_parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::shared_ptr<identifier_node>& get_return_type()
      const {
    return return_type_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_body() const {
    return body_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = std::move(parameters);
  }

  void set_body(std::shared_ptr<body_node> body) { body_ = std::move(body); }
  void set_return_type(std::shared_ptr<identifier_node> return_type) {
    return_type_ = std::move(return_type);
  }
};

class constructor_node : public member_node {
  std::shared_ptr<parameters_node> parameters_;
  std::shared_ptr<body_node> body_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<parameters_node>& get_parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_body() const {
    return body_;
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = std::move(parameters);
  }

  void set_body(std::shared_ptr<body_node> body) { body_ = std::move(body); }
};

class parameters_node : public ast_node {
  std::vector<std::shared_ptr<parameter_node>> parameters_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<parameter_node>>&
  getParameters() const {
    return parameters_;
  }

  void add_parameter(std::shared_ptr<parameter_node> parameter) {
    parameters_.push_back(std::move(parameter));
  }
};

class parameter_node : public ast_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> class_name_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<class_name_node>& get_class_name() const {
    return class_name_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
  }
  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = std::move(class_name);
  }
};

class body_node : public ast_node {
 private:
  std::vector<std::shared_ptr<ast_node>> nodes_;

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<ast_node>>& get_nodes()
      const noexcept {
    return nodes_;
  }

  void set_nodes(
      const std::vector<std::shared_ptr<ast_node>>& nodes) noexcept {
    nodes_ = nodes;
  }

  void add_node(std::shared_ptr<ast_node> node) noexcept {
    nodes_.push_back(std::move(node));
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
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier()
      const noexcept {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression()
      const noexcept {
    return expression_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) noexcept {
    identifier_ = std::move(identifier);
  }

  void set_expression(std::shared_ptr<expression_node> expression) noexcept {
    expression_ = std::move(expression);
  }

  bool validate() override { return true; }

  void generate() override {}
};

class while_loop_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<body_node> body_;

 public:
  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression()
      const noexcept {
    return expression_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_body_node()
      const noexcept {
    return body_;
  }

  void set_expression(std::shared_ptr<expression_node> expression) noexcept {
    expression_ = std::move(expression);
  }

  void set_body_node(std::shared_ptr<body_node> body_node) noexcept {
    body_ = std::move(body_node);
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
  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression()
      const noexcept {
    return expression_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_true_body()
      const noexcept {
    return true_body_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_false_body()
      const noexcept {
    return false_body_;
  }

  void set_expression(std::shared_ptr<expression_node> expression) noexcept {
    expression_ = std::move(expression);
  }

  void set_true_body(std::shared_ptr<body_node> true_body) noexcept {
    true_body_ = std::move(true_body);
  }

  void set_false_body(std::shared_ptr<body_node> false_body) noexcept {
    false_body_ = std::move(false_body);
  }

  bool validate() override { return true; }

  void generate() override {}
};

class return_statement_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;

 public:
  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression()
      const noexcept {
    return expression_;
  }

  void set_expression(std::shared_ptr<expression_node> expression) noexcept {
    expression_ = std::move(expression);
  }

  bool validate() override { return true; }

  void generate() override {}
};

class expression_node : public statement_node {
 private:
  std::shared_ptr<primary_node> primary_;
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<arguments_node> arguments_;

 public:
  [[nodiscard]] const std::shared_ptr<primary_node>& get_primary()
      const noexcept {
    return primary_;
  }

  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier()
      const noexcept {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<arguments_node>& get_arguments()
      const noexcept {
    return arguments_;
  }

  void set_primary(std::shared_ptr<primary_node> primary) noexcept {
    primary_ = std::move(primary);
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) noexcept {
    identifier_ = std::move(identifier);
  }

  void set_arguments(std::shared_ptr<arguments_node> arguments) noexcept {
    arguments_ = std::move(arguments);
  }

  void get_identifier(std::shared_ptr<identifier_node> identifier) noexcept {
    identifier_ = std::move(identifier);
  }

  void get_arguments(std::shared_ptr<arguments_node> arguments) noexcept {
    arguments_ = std::move(arguments);
  }

  bool validate() override { return true; }

  void generate() override {}
};

class arguments_node : public ast_node {
 private:
  std::vector<std::shared_ptr<expression_node>> expressions_;

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<expression_node>>&
  get_expressions() const noexcept {
    return expressions_;
  }

  void set_expressions(const std::vector<std::shared_ptr<expression_node>>&
                           expression) noexcept {
    expressions_ = expression;
  }

  void add_expression(std::shared_ptr<expression_node> expression) {
    expressions_.push_back(std::move(expression));
  } 

  bool validate() override { return true; }

  void generate() override {}
};

template <typename T>
class literal_node : public primary_node {
  T value_;
  bool validate() override { return true; }

  void generate() override {}

 public:
  literal_node() {}
  literal_node(T value) : value_(value) {}
  void set_value(const T& value) { value_ = value; }

  const T& value() const { return value_; }
};

class this_node : public primary_node {
  bool validate() override { return true; }

  void generate() override {}
};

class null_node : public primary_node {
  bool validate() override { return true; }

  void generate() override {}
};
}  // namespace details
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./../lexical-analyzer/token.h"
#include "lexical-analyzer/token-code.h"

namespace scope {
class scope;
}

namespace visitor {
class visitor;
}

namespace error_handling {
class error_handling;
}

namespace details {
using token::span;
const span zero_span{std::string::npos, std::string::npos, std::string::npos,
                     std::string::npos};

struct meta {
  std::string name_;
  token_id token_;
  token::span span_{zero_span};

  meta() = default;

  explicit meta(std::string name, const token_id& token, const span& span)
      : name_{std::move(name)}, token_{token}, span_{span} {}

  meta(const meta& meta)
      : name_{meta.name_}, token_{meta.token_}, span_{meta.span_} {}

  bool operator==(const meta& other) const = default;
};
}  // namespace details

namespace details {
class identifier_node;
class primary_node;
class class_name_node;
class parameter_node;
class parameters_node;
class body_node;
class class_node;
class program_node;
class expression_node;
class variable_node;
class method_node;
class constructor_node;
class assignment_node;
class while_loop_node;
class if_statement_node;
class return_statement_node;
class arguments_node;
class this_node;
class null_node;
class base_node;

inline void merge_in_left(token::span& l, const token::span& r) {
  if (l == zero_span) {
    l = r;
  } else {
    l.merge(r);
  }
}

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

class ast_node {
 protected:
  meta meta_info_;
  virtual bool validate() = 0;
  virtual void generate() = 0;

 public:
  explicit ast_node() : meta_info_{meta{}} {}
  explicit ast_node(const meta& meta_info) : meta_info_{meta_info} {}

  [[nodiscard]] const meta& get_meta_info() const noexcept {
    return meta_info_;
  }
  void set_meta(const meta& meta) noexcept { meta_info_ = meta; }

  virtual void visit(visitor::visitor*) = 0;
  virtual void print() = 0;
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

  void set_name(const token::identifier& i) noexcept {
    set_name(i.get_value());
    meta_info_ = meta(i.get_value(), i.get_token_id(), i.get_span());
  }

  [[nodiscard]] const std::string& get_name() const noexcept { return name_; }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override { std::cout << name_; }
};

class primary_node : public ast_node {
 private:
  std::string representation_;

 public:
  void visit(visitor::visitor* v) override;

  void print() override { std::cout << representation_; }
};

class class_name_node : public primary_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> generic_;
  std::shared_ptr<scope::scope> scope_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    fill(identifier_);
    fill(generic_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<class_name_node>& get_generic() const {
    return generic_;
  }

  [[nodiscard]] const std::shared_ptr<scope::scope>& get_scope() const {
    return scope_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
    fill();
  }

  void set_generic(std::shared_ptr<class_name_node> generic) {
    generic_ = std::move(generic);
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    if (identifier_) {
      identifier_->print();
    }
    if (generic_) {
      std::cout << "[";
      generic_->print();
      std::cout << "]";
    }
  }

  std::string mangle_class_name() const {
    auto name = identifier_->get_name();
    return std::to_string(name.size()) + name;
  }
};

class type_node : public ast_node {
  using value_type = std::shared_ptr<class_name_node>;
  value_type type_;

  bool validate() override { return true; }

  void visit(visitor::visitor* v) override;

  void generate() override {}

 public:
  type_node(value_type class_name) : type_(std::move(class_name)) {}
  void set_type(value_type type) { type_ = std::move(type); }

  const value_type& get_class_name() const { return type_; }

  bool operator==(const type_node& other) {
    return type_->mangle_class_name() == other.type_->mangle_class_name();
  }

  std::string get_type() const { return type_->mangle_class_name(); }

  void print() override { type_->print(); }

  void register_class(std::shared_ptr<class_node> clazz) {
    types_.insert({get_type(), clazz});
  }

  static std::shared_ptr<class_node> find(const std::string& class_name) {
    if (types_.contains(class_name)) return types_[class_name];

    return nullptr;
  }

  inline static std::unordered_map<std::string, std::shared_ptr<class_node>>
      types_ = {};
};

class parameter_node : public ast_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> class_name_;
  std::shared_ptr<scope::scope> scope_;
  std::shared_ptr<type_node> type_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    fill(identifier_);
    fill(type_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<type_node>& get_type() const {
    return type_;
  }

  [[nodiscard]] const std::shared_ptr<class_name_node>& get_class_name() const {
    return class_name_;
  }

  [[nodiscard]] const std::shared_ptr<scope::scope> get_scope() const {
    return scope_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
    fill();
  }
  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    type_ = std::make_shared<type_node>(std::move(class_name));
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    if (identifier_) {
      identifier_->print();
      std::cout << ": ";
      type_->print();
    }
  }

  std::string mangle_param() const { return type_->get_type(); }
};

class parameters_node : public ast_node {
  std::vector<std::shared_ptr<parameter_node>> parameters_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    for (const auto& p : parameters_) {
      if (p) {
        fill(p);
      }
    }
  }

  void fill(const std::shared_ptr<parameter_node>& node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<parameter_node>>&
  get_parameters() const {
    return parameters_;
  }

  void add_parameter(std::shared_ptr<parameter_node> parameter) {
    fill(parameter);
    parameters_.push_back(std::move(parameter));
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    for (std::size_t i = 0, size = parameters_.size(); i < size; i++) {
      parameters_[i]->print();
      if (i != size - 1) {
        std::cout << ", ";
      }
    }
  }

  [[nodiscard]] std::string mangle_parameters() const {
    if (parameters_.empty()) {
      return "v";
    }

    std::string mangledParameters;
    for (auto& param : parameters_) {
      mangledParameters.append(param->mangle_param());
    }
    return mangledParameters + "E";
  }
};

class body_node : public ast_node {
 private:
  std::vector<std::shared_ptr<ast_node>> nodes_;

  void fill() {
    meta_info_.span_ = zero_span;
    for (const auto& n : nodes_) {
      if (n) {
        fill(n);
      }
    }
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<ast_node>>& get_nodes()
      const noexcept {
    return nodes_;
  }

  void set_nodes(const std::vector<std::shared_ptr<ast_node>>& nodes) noexcept {
    nodes_ = nodes;
    fill();
  }

  void add_node(std::shared_ptr<ast_node> node) noexcept {
    fill(node);
    nodes_.push_back(std::move(node));
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    for (auto& st : nodes_) {
      st->print();
      std::cout << "\n";
    }
  }
};

class member_node : public ast_node {};

class class_node : public ast_node,
                   public std::enable_shared_from_this<class_node> {
  std::shared_ptr<type_node> class_name_;
  std::shared_ptr<type_node> extends_;
  std::vector<std::shared_ptr<member_node>> members_;
  std::shared_ptr<scope::scope> scope_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    fill(class_name_);
    fill(extends_);
    for (const auto& p : members_) {
      if (p) {
        fill(p);
      }
    }
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] std::shared_ptr<class_name_node> get_class_name()
      const noexcept {
    return class_name_->get_class_name();
  }

  [[nodiscard]] std::shared_ptr<type_node> get_extends() const noexcept {
    return extends_;
  }

  [[nodiscard]] const std::vector<std::shared_ptr<member_node>>& get_members()
      const noexcept {
    return members_;
  }

  [[nodiscard]] std::shared_ptr<scope::scope> get_scope() const noexcept {
    return scope_;
  }

  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = std::make_shared<type_node>(std::move(class_name));
    class_name_->register_class(shared_from_this());
    fill();
  }

  void set_extends(std::shared_ptr<class_name_node> extends) {
    if (extends) extends_ = std::make_shared<type_node>(std::move(extends));
    fill();
  }

  void add_member(std::shared_ptr<member_node> member) {
    fill(member);
    members_.push_back(std::move(member));
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    if (class_name_) {
      std::cout << "class ";
      class_name_->print();
      std::cout << " ";
    }
    if (extends_) {
      std::cout << " extends ";
      extends_->print();
    }
    std::cout << " is\n";

    for (const auto& member : members_) {
      if (member) {
        member->print();
        std::cout << "\n";
      }
    }

    std::cout << "end\n";
  }
};

class program_node : public ast_node {
  std::vector<std::shared_ptr<class_node>> classes_;
  std::shared_ptr<scope::scope> scope_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    for (const auto& c : classes_) {
      if (c) {
        fill(c);
      }
    }
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<class_node>>& get_classes()
      const {
    return classes_;
  }

  void add_class(std::shared_ptr<class_node> class_) {
    fill(class_);
    classes_.push_back(std::move(class_));
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    for (const auto& clazz : classes_) {
      if (clazz) {
        clazz->print();
      }
    }
  }

  std::shared_ptr<scope::scope> get_scope() const noexcept { return scope_; }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }
};

class statement_node : public ast_node {};

class expression_node : public statement_node {
 private:
  std::shared_ptr<primary_node> primary_;
  using value_type = std::vector<std::pair<std::shared_ptr<identifier_node>,
                                           std::shared_ptr<arguments_node>>>;
  value_type expression_values;
  std::shared_ptr<type_node> expression_type_;
  std::shared_ptr<scope::scope> scope_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(primary_);
    for (const auto& ev : expression_values) {
      fill(ev);
    }
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

  void fill(std::pair<std::shared_ptr<identifier_node>,
                      std::shared_ptr<arguments_node>>
                value);

 public:
  [[nodiscard]] const std::shared_ptr<primary_node>& get_primary()
      const noexcept {
    return primary_;
  }

  [[nodiscard]] const value_type& get_expression_values() const noexcept {
    return expression_values;
  }

  [[nodiscard]] const std::shared_ptr<scope::scope>& get_scope()
      const noexcept {
    return scope_;
  }

  void set_primary(std::shared_ptr<primary_node> primary) noexcept {
    primary_ = std::move(primary);
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) noexcept {
    scope_ = std::move(scope);
  }

  void add_value(std::pair<std::shared_ptr<identifier_node>,
                           std::shared_ptr<arguments_node>>
                     value) noexcept {
    fill(value);
    expression_values.push_back(std::move(value));
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override;

  std::shared_ptr<type_node> get_type() { return expression_type_; }
};

class variable_node : public member_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<scope::scope> scope_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(identifier_);
    fill(expression_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

  bool validate() override { return true; }

  void generate() override {}

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression() const {
    return expression_;
  }

  [[nodiscard]] const std::shared_ptr<scope::scope>& get_scope() const {
    return scope_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
    fill();
  }

  void set_expression(std::shared_ptr<expression_node> expression) {
    expression_ = std::move(expression);
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "var ";
    identifier_->print();
    std::cout << " : ";
    expression_->print();
  }
};

class method_node : public member_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<parameters_node> parameters_;
  std::shared_ptr<type_node> return_type_;
  std::shared_ptr<body_node> body_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    fill(identifier_);
    fill(parameters_);
    fill(return_type_);
  }

  void fill(const std::shared_ptr<ast_node>& node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<parameters_node>& get_parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::shared_ptr<type_node>& get_return_type() const {
    return return_type_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_body() const {
    return body_;
  }

  void set_identifier(std::shared_ptr<identifier_node> identifier) {
    identifier_ = std::move(identifier);
    fill();
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = std::move(parameters);
    fill();
  }

  void set_body(std::shared_ptr<body_node> body) {
    body_ = std::move(body);
    fill();
  }
  void set_return_type(std::shared_ptr<class_name_node> return_type) {
    return_type_ = std::make_shared<type_node>(std::move(return_type));
    fill();
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "method ";
    identifier_->print();
    std::cout << "(";
    parameters_->print();
    std::cout << ")";

    if (return_type_) {
      std::cout << " : ";
      return_type_->print();
    }

    std::cout << " is\n";
    body_->print();
    std::cout << "end";
  }

  std::string mangle_method() const {
    return std::to_string(identifier_->get_name().size()) +
           identifier_->get_name() + "E" + parameters_->mangle_parameters();
  }
};

class constructor_node : public member_node {
  std::shared_ptr<parameters_node> parameters_;
  std::shared_ptr<body_node> body_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    if (parameters_ && !parameters_->get_parameters().empty()) {
      fill(parameters_);
    }
  }

  void fill(const std::shared_ptr<ast_node>& node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::shared_ptr<parameters_node>& get_parameters() const {
    return parameters_;
  }

  [[nodiscard]] const std::shared_ptr<body_node>& get_body() const {
    return body_;
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = std::move(parameters);
    fill();
  }

  void set_body(std::shared_ptr<body_node> body) {
    body_ = std::move(body);
    fill();
  }

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "this(";
    parameters_->print();
    std::cout << ") is\n";
    body_->print();
    std::cout << "end";
  }

  std::string mangle_ctr() const {
    return "C" + parameters_->mangle_parameters();
  }
};

class assignment_node : public statement_node {
 private:
  std::shared_ptr<expression_node> lexpression_;
  std::shared_ptr<expression_node> rexpression_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(lexpression_);
    fill(rexpression_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::shared_ptr<expression_node>& get_lexpression()
      const noexcept {
    return lexpression_;
  }

  [[nodiscard]] const std::shared_ptr<expression_node>& get_rexpression()
      const noexcept {
    return rexpression_;
  }

  void set_lexpression(std::shared_ptr<expression_node> lexpression) noexcept {
    lexpression_ = std::move(lexpression);
    fill();
  }

  void set_rexpression(std::shared_ptr<expression_node> rexpression) noexcept {
    rexpression_ = std::move(rexpression);
    fill();
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    lexpression_->print();
    std::cout << " := ";
    rexpression_->print();
  }
};

class while_loop_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<body_node> body_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(expression_);
    fill(body_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

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
    fill();
  }

  void set_body_node(std::shared_ptr<body_node> body_node) noexcept {
    body_ = std::move(body_node);
    fill();
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "while ";
    expression_->print();
    std::cout << " loop\n";
    body_->print();
    std::cout << "end";
  }
};

class if_statement_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<body_node> true_body_;
  std::shared_ptr<body_node> false_body_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(expression_);
    fill(true_body_);
    fill(false_body_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

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
    fill();
  }

  void set_true_body(std::shared_ptr<body_node> true_body) noexcept {
    true_body_ = std::move(true_body);
    fill();
  }

  void set_false_body(std::shared_ptr<body_node> false_body) noexcept {
    false_body_ = std::move(false_body);
    fill();
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "if ";
    expression_->print();
    std::cout << " then\n";
    true_body_->print();
    if (false_body_) {
      std::cout << "else ";
      false_body_->print();
    }
    std::cout << "end";
  }
};

class return_statement_node : public statement_node {
 private:
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<scope::scope> scope_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(expression_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] std::shared_ptr<expression_node> get_expression()
      const noexcept {
    return expression_;
  }

  [[nodiscard]] std::shared_ptr<scope::scope> get_scope() const noexcept {
    return scope_;
  }

  void set_expression(std::shared_ptr<expression_node> expression) noexcept {
    expression_ = std::move(expression);
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "return ";
    if (expression_) {
      expression_->print();
    }
  }
};

class arguments_node : public ast_node {
 private:
  using arguments_holder = std::vector<std::shared_ptr<expression_node>>;
  arguments_holder expressions_;

  void fill() {
    meta_info_.span_ = zero_span;
    for (const auto& e : expressions_) {
      fill(e);
    }
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] const std::vector<std::shared_ptr<expression_node>>&
  get_expressions() const noexcept {
    return expressions_;
  }

  void set_expressions(const std::vector<std::shared_ptr<expression_node>>&
                           expression) noexcept {
    expressions_ = expression;
    fill();
  }

  void add_expression(std::shared_ptr<expression_node> expression) {
    fill(expression);
    expressions_.push_back(std::move(expression));
  }

  arguments_holder get_arguments() const { return expressions_; }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << "(";
    for (std::size_t i = 0, size = expressions_.size(); i < size; i++) {
      expressions_[i]->print();
      if (i != size - 1) {
        std::cout << ", ";
      }
    }
    std::cout << ")";
  }
};

template <typename T>
class literal_node : public primary_node {
  T value_;
  bool validate() override { return true; }

  void generate() override {}

 public:
  literal_node() {}
  literal_node(T value) : value_(value) {}
  literal_node(const token::basic_template_token<T>& value)
      : value_(value.get_value()) {
    meta_info_ = meta("literal", value.get_token_id(), value.get_span());
  }

  void set_value(const T& value) { value_ = value; }

  void set_value(const token::basic_template_token<T>& value) {
    value_ = value.get_value();
    meta_info_ = meta("literal", value.get_token_id(), value.get_span());
  }

  const T& value() const { return value_; }

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << std::boolalpha << value_ << std::noboolalpha;
  }
};

class this_node : public primary_node {
 public:
  this_node(const token::keyword& i) {
    meta_info_ = meta(i.get_value(), i.get_token_id(), i.get_span());
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override { std::cout << "this"; }
};

class null_node : public primary_node {
 public:
  null_node(const token::keyword& i) {
    meta_info_ = meta(i.get_value(), i.get_token_id(), i.get_span());
  }

  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override { std::cout << "null"; }
};

class base_node : public primary_node {
  std::shared_ptr<arguments_node> arguments_;

  bool validate() override { return true; }

  void generate() override {}

  void fill() {
    meta_info_.span_ = zero_span;
    fill(arguments_);
  }

  void fill(std::shared_ptr<ast_node> node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  void set_arguments(std::shared_ptr<arguments_node> arguments) noexcept {
    arguments_ = std::move(arguments);
    fill();
  }

  void get_arguments(std::shared_ptr<arguments_node> arguments) noexcept {
    arguments_ = std::move(arguments);
  }

  void visit(visitor::visitor* v) override;

  void print() override { std::cout << "base"; }
};

inline void expression_node::print() {
  primary_->print();
  for (auto& [identifier, arguments] : expression_values) {
    if (identifier) {
      std::cout << ".";
      identifier->print();
    }
    if (arguments) arguments->print();
  }
}

inline void expression_node::fill(
    std::pair<std::shared_ptr<identifier_node>, std::shared_ptr<arguments_node>>
        value) {
  fill(value.first);
  fill(value.second);
}
}  // namespace details
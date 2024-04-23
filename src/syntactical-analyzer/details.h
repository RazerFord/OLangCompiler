#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "lexical-analyzer/token-code.h"
#include "lexical-analyzer/token.h"

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
class variable_call;
template <typename T>
class literal_node;

inline void merge_in_left(token::span& l, const token::span& r) {
  if (l == zero_span) {
    l = r;
  } else {
    l.merge(r);
  }
}

class ast_node : public std::enable_shared_from_this<ast_node> {
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
};

enum class type_id {
  Undefined,
  Custom,
  Integer,
  Real,
  Boolean,
  String,
  Void,
  i,
  f,
  b,
  Null,
  BaseCall,
  Printf,
};

class type_node : public ast_node {
  type_id id_;
  using value_type = std::shared_ptr<class_name_node>;
  value_type type_;

  bool validate() override { return true; }

  void visit(visitor::visitor* v) override;

  void generate() override {}

 public:
  type_node() = default;
  explicit type_node(type_id id) : id_(id) {}
  explicit type_node(value_type class_name) : type_(std::move(class_name)) {
    id_ = type_id::Custom;
  }
  void set_type(value_type type) {
    type_ = std::move(type);
    id_ = type_id::Custom;
  }

  value_type get_class_name() const { return type_; }

  bool operator==(const type_node& other) {
    return mangle_name(type_->get_identifier()->get_name()) ==
           mangle_name(other.type_->get_identifier()->get_name());
  }

  std::string type() const {
    if (type_) return mangle_name(type_->get_identifier()->get_name());
    return mangle_name(id_);
  }

  std::string simple_type() const {
    if (type_) return type_->get_identifier()->get_name();
    return type_id_to_name(id_);
  }

  void print() override {
    if (type_) type_->print();
  }

  void register_class(std::shared_ptr<class_node> clazz) {
    types_.insert({type(), clazz});
  }

  static std::shared_ptr<class_node> find(const std::string& class_name) {
    if (types_.contains(class_name)) return types_[class_name];

    return nullptr;
  }

  static std::string mangle_name(const std::string& name) {
    return std::to_string(name.size()) + name;
  }

  static std::string mangle_name(type_id id) { return type_id_str[id]; }

  static std::string type_id_to_name(type_id id) {
    return type_id_simple_str[id];
  }

  inline static std::unordered_map<std::string, std::shared_ptr<class_node>>
      types_ = {};

  inline static const std::string IntegerT = "Integer";
  inline static const std::string RealT = "Real";
  inline static const std::string BooleanT = "Boolean";
  inline static const std::string AnyT = "Any";
  inline static const std::string intT = "int";
  inline static const std::string realT = "real";
  inline static const std::string booleanT = "bool";
  inline static const std::string voidT = "void";
  inline static const std::string baseT = "base";

  inline static std::unordered_map<type_id, std::string> type_id_str = {
      {type_id::Integer, mangle_name(IntegerT)},
      {type_id::Real, mangle_name(RealT)},
      {type_id::Boolean, mangle_name(BooleanT)},
      {type_id::i, mangle_name(intT)},
      {type_id::f, mangle_name(realT)},
      {type_id::b, mangle_name(booleanT)},
      {type_id::Void, mangle_name(voidT)},
      {type_id::BaseCall, mangle_name(baseT)},
      {type_id::Undefined, mangle_name("/Undefined")},
  };

  inline static std::unordered_map<type_id, std::string> type_id_simple_str = {
      {type_id::Integer, IntegerT}, {type_id::Real, RealT},
      {type_id::Boolean, BooleanT}, {type_id::i, intT},
      {type_id::f, realT},          {type_id::b, booleanT},
      {type_id::BaseCall, baseT},
      {type_id::Void, voidT},       {type_id::Undefined, "/Undefined"},
  };
};

class parameter_node : public ast_node {
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<class_name_node> class_name_;
  std::shared_ptr<scope::scope> scope_;
  std::shared_ptr<type_node> type_;
  llvm::Value* param_value_;

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
  void set_param_value(llvm::Value* param_value) { param_value_ = param_value; }

  llvm::Value* get_param_value() const noexcept { return param_value_; }

  void visit(visitor::visitor* v) override;

  void print() override {
    if (identifier_) {
      identifier_->print();
      std::cout << ": ";
      type_->print();
    }
  }

  std::string mangle_param() const { return type_->type(); }
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
  [[nodiscard]] std::vector<std::shared_ptr<parameter_node>>& get_parameters() {
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

class class_node : public ast_node {
  std::shared_ptr<type_node> class_name_;
  std::shared_ptr<type_node> extends_;
  std::vector<std::shared_ptr<member_node>> members_;
  std::shared_ptr<scope::scope> scope_;
  llvm::Type* class_type_;

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

  void fill(const std::shared_ptr<ast_node>& node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

 public:
  [[nodiscard]] std::shared_ptr<class_name_node> get_class_name()
      const noexcept {
    return class_name_->get_class_name();
  }

  [[nodiscard]] std::shared_ptr<class_name_node> get_extends() const {
    if (extends_) return extends_->get_class_name();
    return {};
  }

  std::shared_ptr<type_node> get_type() const { return class_name_; }

  [[nodiscard]] const std::vector<std::shared_ptr<member_node>>& get_members()
      const noexcept {
    return members_;
  }

  [[nodiscard]] std::shared_ptr<scope::scope> get_scope() const noexcept {
    return scope_;
  }

  void set_class_name(std::shared_ptr<class_name_node> class_name) {
    class_name_ = std::make_shared<type_node>(std::move(class_name));
    class_name_->register_class(
        std::dynamic_pointer_cast<class_node>(shared_from_this()));
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

  void set_class_type(llvm::Type* class_type) { class_type_ = class_type; }

  llvm::Type* get_class_type() const noexcept { return class_type_; }

  std::shared_ptr<method_node> find_method(
      const std::shared_ptr<identifier_node>& method_name,
      const std::shared_ptr<arguments_node>& args,
      error_handling::error_handling& error_handler);

  std::shared_ptr<constructor_node> find_ctr(
      error_handling::error_handling& error_handler);
  template <typename T>
  std::shared_ptr<constructor_node> find_ctr(
      std::shared_ptr<literal_node<T>> literal);

  std::shared_ptr<constructor_node> find_ctr(
      const std::shared_ptr<arguments_node>& args,
      error_handling::error_handling& error_handler);

  std::shared_ptr<variable_node> find_var_member(
      const std::shared_ptr<identifier_node>& var_name,
      error_handling::error_handling& error_handler);

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
  std::unordered_map<std::string, std::unordered_map<std::string, bool>>
      type_casting_;

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

  void set_type_casting(
      std::unordered_map<std::string, std::unordered_map<std::string, bool>>
          type_casting) {
    type_casting_ = std::move(type_casting);
  }

  const std::unordered_map<std::string, std::unordered_map<std::string, bool>>&
  get_type_casting() const noexcept {
    return type_casting_;
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

class expression_ext : public ast_node {
  llvm::Value* value_;

 public:
  virtual ~expression_ext() {}
  virtual std::shared_ptr<type_node> get_type() const noexcept = 0;

  virtual void set_value(llvm::Value* value) { value_ = value; }

  virtual llvm::Value* get_value() const noexcept { return value_; }
};

class expression_node : public statement_node {
  llvm::Value* value_;

 private:
  using value_type = std::vector<std::pair<std::shared_ptr<identifier_node>,
                                           std::shared_ptr<arguments_node>>>;

  std::shared_ptr<primary_node> primary_;
  value_type expression_values;
  std::shared_ptr<expression_ext> final_object_;
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

  std::shared_ptr<expression_ext> this_type_checking(
      const std::shared_ptr<this_node>&,
      error_handling::error_handling& error_handler);

  std::shared_ptr<expression_ext> base_checking(
      std::shared_ptr<base_node>,
      error_handling::error_handling& error_handler);
  std::shared_ptr<expression_ext> constr_call_checking(
      std::shared_ptr<ast_node> constr_call, std::shared_ptr<class_node> clazz,
      error_handling::error_handling& error_handler);
  std::shared_ptr<expression_ext> printf_checking(
      error_handling::error_handling& error_handler);

 public:

  expression_node() {}
  expression_node(std::shared_ptr<primary_node> primary): primary_(primary) {}
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

  void set_value(llvm::Value* value) { value_ = value; }

  llvm::Value* get_value() const noexcept { return value_; }

  std::shared_ptr<expression_ext> get_final_object() const noexcept {
    return final_object_;
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

  std::shared_ptr<type_node> get_type() {
    if (final_object_) return final_object_->get_type();
    return nullptr;
  }

  std::shared_ptr<type_node> get_type(
      error_handling::error_handling& error_handler) {
    get_object(error_handler);
    if (final_object_) return final_object_->get_type();
    return nullptr;
  }

  std::shared_ptr<expression_ext> get_object(
      error_handling::error_handling& error_handler);
};

class variable_node : public member_node {
  int index_ = -1; // -1 for local variable
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;
  std::shared_ptr<scope::scope> scope_;
  llvm::Value* value_;

  void fill() {
    meta_info_.span_ = zero_span;
    fill(identifier_);
    fill(expression_);
  }

  void fill(const std::shared_ptr<ast_node>& node) {
    if (node) {
      merge_in_left(meta_info_.span_, node->get_meta_info().span_);
    }
  }

  bool validate() override { return true; }

  void generate() override {}

 public:
  int get_index() const noexcept { return index_; }

  void set_index(int index) noexcept { index_ = index; }

  [[nodiscard]] const std::shared_ptr<identifier_node>& get_identifier() const {
    return identifier_;
  }

  [[nodiscard]] const std::shared_ptr<expression_node>& get_expression() const {
    return expression_;
  }

  [[nodiscard]] const std::shared_ptr<scope::scope>& get_scope() const {
    return scope_;
  }
  std::shared_ptr<type_node> get_type() { return expression_->get_type(); }

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
    if (expression_) expression_->set_scope(scope_);
  }

  void set_value(llvm::Value* value) { value_ = value; }

  llvm::Value* get_value() const noexcept { return value_; }

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
  std::shared_ptr<type_node> return_type_ =
      std::make_shared<type_node>(type_id::Void);
  std::shared_ptr<body_node> body_;
  llvm::Function* method_value_;

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

  [[nodiscard]] std::shared_ptr<type_node> get_return_type() {
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

  void set_method_value(llvm::Function* method) { method_value_ = method; }

  llvm::Function* get_method_value() const noexcept { return method_value_; }

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
  std::shared_ptr<scope::scope> scope_;
  llvm::Function* constr_value_;

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
  [[nodiscard]] std::shared_ptr<parameters_node> get_parameters() const {
    return parameters_;
  }

  [[nodiscard]] std::shared_ptr<body_node> get_body() const { return body_; }

  [[nodiscard]] std::shared_ptr<scope::scope> get_scope() const {
    return scope_;
  }

  void set_parameters(std::shared_ptr<parameters_node> parameters) {
    parameters_ = std::move(parameters);
    fill();
  }

  void set_body(std::shared_ptr<body_node> body) {
    body_ = std::move(body);
    fill();
  }

  void set_scope(std::shared_ptr<scope::scope> scope) {
    scope_ = std::move(scope);
  }

  void set_constr_type(llvm::Function* constr_value) {
    constr_value_ = constr_value;
  }

  llvm::Function* get_constr_value() const noexcept { return constr_value_; }

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
    if (expression_) expression_->set_scope(scope_);
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

class literal_base_node : public primary_node {
 public:
  virtual std::shared_ptr<expression_ext> literal_expression_handle() = 0;
};

template <typename T>
class literal_node : public literal_base_node {
  T value_;
  bool validate() override { return true; }

  void generate() override {}

 public:
  literal_node() = default;
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

  std::shared_ptr<expression_ext> literal_expression_handle() override;

  void visit(visitor::visitor* v) override;

  void print() override {
    std::cout << std::boolalpha << value_ << std::noboolalpha;
  }
};

class this_node : public primary_node {
 public:
  explicit this_node(const token::keyword& i) {
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

class void_node: public primary_node {
 public:
  bool validate() override { return true; }

  void generate() override {}

  void visit(visitor::visitor* v) override;

  void print() override { std::cout << "null"; }
};

class base_node : public primary_node {
  bool validate() override { return true; }

  void generate() override {}

 public:
  base_node(const token::keyword& i) {
    meta_info_ = meta(i.get_value(), i.get_token_id(), i.get_span());
  }
  void visit(visitor::visitor* v) override;

  void print() override { std::cout << "base"; }
};

class printf_call : public expression_ext {
  std::string formatted_str_;
  std::vector<std::shared_ptr<expression_ext>> arguments_;
  std::shared_ptr<type_node> type_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  printf_call(std::string formatted_str,
              const std::vector<std::shared_ptr<expression_ext>>& args)
      : formatted_str_(std::move(formatted_str)), arguments_(args), type_(std::make_shared<type_node>(type_id::Printf)) {}

  printf_call(std::string formatted_str, std::vector<std::shared_ptr<expression_ext>>&& args)
      : formatted_str_(std::move(formatted_str)), arguments_(args), type_(std::make_shared<type_node>(type_id::Printf)) {}

  std::shared_ptr<type_node> get_type() const noexcept override {
    return type_;
  }

  std::string get_formatted_str() const noexcept {
    return formatted_str_;
  }

  std::vector<std::shared_ptr<expression_ext>> get_arguments() const noexcept {
    return arguments_;
  }

  void visit(visitor::visitor* v) override;
  void print() override {};
};

class variable_call : public expression_ext {
  std::shared_ptr<ast_node> variable_;
  std::shared_ptr<type_node> type_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  variable_call() {}
  variable_call(std::shared_ptr<ast_node> variable,
                std::shared_ptr<type_node> type)
      : variable_(variable), type_(type) {}

  void set_object(std::shared_ptr<ast_node> caller_object) {
    variable_ = std::move(caller_object);
  }

  void set_type(std::shared_ptr<type_node> type) { type_ = std::move(type); }

  std::shared_ptr<ast_node> get_variable() const noexcept { return variable_; }
  std::shared_ptr<type_node> get_type() const noexcept override { return type_; }
  void visit(visitor::visitor* v) override;

  void print() override{};
};

class constructor_call : public expression_ext {
  std::weak_ptr<class_node> class_;
  std::weak_ptr<constructor_node> constr_;
  std::vector<std::shared_ptr<expression_ext>> arguments_;
  std::shared_ptr<type_node> type_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  constructor_call() {}
  constructor_call(const constructor_call& other)
      : class_(other.class_),
        constr_(other.constr_),
        arguments_(other.arguments_),
        type_(other.type_) {}
  constructor_call(std::shared_ptr<class_node> clazz,
                   std::shared_ptr<constructor_node> constr,
                   std::vector<std::shared_ptr<expression_ext>> args,
                   std::shared_ptr<type_node> type)
      : class_(clazz), constr_(constr), arguments_(args), type_(type) {}

  std::shared_ptr<type_node> get_type() const noexcept override { return type_; }

  // todo override
  void visit(visitor::visitor* v) override;

  void print() override{};

  std::shared_ptr<constructor_node> get_constructor() const noexcept {
    return constr_.lock();
  }
  std::vector<std::shared_ptr<expression_ext>> get_arguments() const noexcept {
    return arguments_;
  }
};

class method_call : public expression_ext {
  std::weak_ptr<class_node> clazz_;
  std::weak_ptr<method_node> method_;
  std::vector<std::shared_ptr<expression_ext>> arguments_;

  llvm::Value* owner_value_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  method_call() {}
  method_call(std::shared_ptr<class_node> clazz,
              std::shared_ptr<method_node> method,
              std::vector<std::shared_ptr<expression_ext>> args)
      : clazz_(clazz), method_(method), arguments_(args) {}

  void set_owner_value(llvm::Value* owner_value) { owner_value_ = owner_value; }

  llvm::Value* get_owner_value() const noexcept { return owner_value_; }

  std::shared_ptr<type_node> get_type() const noexcept override {
    return method_.lock()->get_return_type();
  }
  std::shared_ptr<method_node> get_method() const noexcept {
    return method_.lock();
  }

  std::vector<std::shared_ptr<expression_ext>> get_arguments() const noexcept {
    return arguments_;
  }

  void visit(visitor::visitor* v) override;

  void print() override{};
};

class member_call : public expression_ext {
  std::shared_ptr<expression_ext> object_;
  std::shared_ptr<expression_ext> member_ref_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  member_call() {}
  member_call(std::shared_ptr<expression_ext> object,
              std::shared_ptr<expression_ext> member)
      : object_(object), member_ref_(member) {}

  void set_object(std::shared_ptr<expression_ext> object) {
    object_ = std::move(object);
  }

  void set_member(std::shared_ptr<expression_ext> member) {
    member_ref_ = std::move(member);
  }

  std::shared_ptr<expression_ext> get_object() const noexcept {
    return object_;
  }

  std::shared_ptr<expression_ext> get_member() const noexcept {
    return member_ref_;
  }

  std::shared_ptr<type_node> get_type() const noexcept override {
    return member_ref_->get_type();
  }
  void visit(visitor::visitor* v) override;

  void print() override{};
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
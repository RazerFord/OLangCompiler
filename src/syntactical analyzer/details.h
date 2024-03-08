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
class expression_node;
class statement_node;
class parameters_node;
class parameter_node;

class identifier_node : public ast_node {
 private:
  std::string name_;

 public:
  void setName(const std::string& name) { name_ = name; }

  std::string getName() { return name_; }

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

  bool validate() override { return true; }

  void generate() override {}
};

class constructor_node : public member_node {
  bool validate() override { return true; }

  void generate() override {}
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
  bool validate() override { return true; }

  void generate() override {}
};

class statement_node : public ast_node {};

class assignment : public statement_node {
 private:
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression_node> expression_;

 public:
  bool validate() override { return true; }

  void generate() override {}
};

class whileloop_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class ifstatement_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class returnstatement_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class expression_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class primary_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};
}  // namespace details
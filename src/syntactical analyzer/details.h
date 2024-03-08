#pragma once

#include <memory>
#include <string>
#include <vector>

#include "lexical-analyzer/token.h"

namespace details {
enum class ast_token {
  Program,
  Class,
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
  bool validate() override { return true; }

  void generate() override {}
};

class program_node : public ast_node {
  std::vector<std::shared_ptr<class_node>> class_children_;

  bool validate() override { return true; }

  void generate() override {}

 public:
  void add_class(std::shared_ptr<class_node> class_) {
    class_children_.push_back(class_);
  }
};

class Program : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Class : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Member : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class variable_node : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Method : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Constructor : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Parameters : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Parameter : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class statement;

class body_node : public ast_node {
 private:
  std::vector<std::shared_ptr<variable_node>> variables_;
  std::vector<std::shared_ptr<statement>> statements_;

 public:
  bool validate() override { return true; }

  void generate() override {}
};

class statement_node : public ast_node {
 public:
  bool validate() override { return true; }

  void generate() override {}
};

class assignment : public statement_node {
 private:
  std::shared_ptr<identifier_node> identifier_;
  std::shared_ptr<expression> expression_;

 public:
  bool validate() override { return true; }

  void generate() override {}
};

class WhileLoop : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class IfStatement : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class ReturnStatement : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Expression : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};

class Primary : public ast_node {
  bool validate() override { return true; }

  void generate() override {}
};
}  // namespace details
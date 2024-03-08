#pragma once

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
  Primary
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

}  // namespace details
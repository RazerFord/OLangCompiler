#pragma once

#include <string>

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

struct span {};

struct meta {
  std::string name_;
  ast_token token_;
  span span_;
};
}  // namespace details
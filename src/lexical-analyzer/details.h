#pragma once

#include <string>

#include "token.h"

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
}  // namespace details
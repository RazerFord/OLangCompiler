#pragma once

#include <memory>
#include <ranges>
#include <vector>

#include "lexical-analyzer/token.h"
namespace tree {
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

using token_vector = std::vector<std::unique_ptr<token::token>>;

class node {
 private:
  std::vector<std::shared_ptr<node>> children_{};

 public:
  ast_token token_;

  void add_child(const std::shared_ptr<node>& child) {
    children_.push_back(child);
  }
};

class ast {
 private:
  std::shared_ptr<node> root_;

 public:
  ast(const std::shared_ptr<node>& root) : root_{root} {}
};

namespace {
inline node make_variable(const token_vector& tokens, int& first_token) {
  return node();
}

inline node make_constructor(const token_vector& tokens, int& first_token) {
  return node();
}

inline node make_member(const token_vector& tokens, int& first_token) {
  return node();
}

inline node make_class(const token_vector& tokens, int& first_token) {
  return node();
}
}  // namespace

inline ast make_ast(const token_vector& tokens) {
  // TODO: let's filter space
  auto program = std::make_shared<node>(ast_token::Program);

  for (int i = 0; i < tokens.size(); i++) {
    auto child = std::make_shared<node>(make_class(tokens, i));
    program->add_child(child);
  }
  return ast(program);
}
}  // namespace tree

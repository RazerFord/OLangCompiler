#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

#include "./../lexical-analyzer/token-code.h"
#include "./../lexical-analyzer/token.h"
#include "details.h"

namespace tree {
using token_vector = std::vector<std::unique_ptr<token::token>>;
using namespace details;

class ast {
 private:
  std::shared_ptr<program_node> root_;

 public:
  ast(const std::shared_ptr<program_node>& root) : root_{root} {}
};

namespace {
template <class T>
struct result {
  T value;
};

class ast_parser {
 private:
  const token_vector& tokens_;

 public:
  ast_parser(const token_vector& token) : tokens_{token} {}

  inline result<std::shared_ptr<class_name_node>> parse_class_name(
      std::size_t& first_token) {
    class_name_node class_name;

    switch (tokens_[first_token]->get_token_id()) {
      case token_id::Class: {
      }

      default: {
        std::cout << "expected class name";
      }
    }

    return {.value = {nullptr}};
  }

  inline result<std::shared_ptr<class_name_node>> parse_extends(
      std::size_t& first_token) {
    class_name_node class_name;

    for (std::size_t i = first_token; i < tokens_.size(); i++) {
      switch (tokens_[i]->get_token_id()) {
        case token_id::NewLine: {
          continue;
        }

        case token_id::Class: {
          parse_class_name(i);
        }

        default: {
          std::cout << "expected class declaration";
        }
      }
    }

    return {.value = {nullptr}};
  }

  inline result<std::shared_ptr<class_node>> parse_class(
      std::size_t& first_token) {
    class_node instance;

    instance.set_class_name(parse_class_name(++first_token).value);
    instance.set_class_name(parse_extends(++first_token).value);

    return {.value = {nullptr}};
  }

  inline result<std::shared_ptr<program_node>> parse_program(
      const std::size_t& first_token) {
    program_node program;

    for (std::size_t i = first_token; i < tokens_.size(); i++) {
      switch (tokens_[i]->get_token_id()) {
        case token_id::NewLine: {
          continue;
        }

        case token_id::Class: {
          program.add_class(parse_class(++i).value);
        }

        default: {
          std::cout << "expected class declaration";
        }
      }
    }

    return {.value = {nullptr}};
  }
};
}  // namespace

inline ast make_ast(token_vector& tokens) {
  std::erase_if(
      tokens, [](auto& tok) { return tok->get_token_id() != token_id::Space; });

  ast_parser parser(tokens);

  return ast(parser.parse_program(0).value);
}
}  // namespace tree
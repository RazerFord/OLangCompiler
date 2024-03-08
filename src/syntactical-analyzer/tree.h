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

  inline result<std::shared_ptr<identifier_node>> parse_identifier(
      std::size_t& first_token);

  inline result<std::shared_ptr<class_name_node>> parse_class_name(
      std::size_t& first_token);

  inline result<std::shared_ptr<class_name_node>> parse_extends(
      std::size_t& first_token);

  inline result<std::shared_ptr<class_node>> parse_class(
      std::size_t& first_token);

  inline result<std::shared_ptr<program_node>> parse_program(
      const std::size_t& first_token);

 public:
  ast_parser(const token_vector& token) : tokens_{token} {}

  inline result<std::shared_ptr<program_node>> parse_program() {
    return parse_program(0);
  }
};

inline result<std::shared_ptr<identifier_node>> ast_parser::parse_identifier(
    std::size_t& first_token) {
  auto id_node = std::make_shared<identifier_node>();

  auto& tok = tokens_[first_token];

  using ptr_tok_id = token::identifier*;
  ptr_tok_id id = nullptr;

  if ((tok->get_token_id() == token_id::Identifier) &&
      (id = dynamic_cast<ptr_tok_id>(tok.get()))) {
    id_node->set_name(id->get_value());
    std::cout << "[ INFO ] class name identifier: " << id->get_value()
              << std::endl;
    return {.value = id_node};
  } else {
    std::cout << "[ ERROR ] expected identifier of class, but was: "
              << token_id_to_string(tok->get_token_id()) << std::endl;
    return {nullptr};
  }
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_class_name(
    std::size_t& first_token) {
  auto class_name = std::make_shared<class_name_node>();

  class_name->set_identifier(parse_identifier(first_token).value);
  // class_name->set_generic(parse_identifier(first_token).value);

  return {class_name};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_extends(
    std::size_t& first_token) {
  class_name_node class_name;

  for (std::size_t i = first_token; i < tokens_.size(); i++) {
    switch (tokens_[i]->get_token_id()) {
      case token_id::NewLine: {
        continue;
      }

      case token_id::Class: {
        parse_class_name(i);
        parse_extends(i);
      }

      default: {
        std::cout << "[ ERROR ] expected class declaration" << std::endl;
      }
    }
  }

  return {nullptr};
}

inline result<std::shared_ptr<class_node>> ast_parser::parse_class(
    std::size_t& first_token) {
  class_node instance;

  instance.set_class_name(parse_class_name(first_token).value);
  instance.set_class_name(parse_extends(++first_token).value);

  return {nullptr};
}

inline result<std::shared_ptr<program_node>> ast_parser::parse_program(
    const std::size_t& first_token) {
  program_node program;

  for (std::size_t i = first_token; i < tokens_.size(); i++) {
    auto& tok = tokens_[i];
    switch (tok->get_token_id()) {
      case token_id::NewLine: {
        continue;
      }

      case token_id::Class: {
        program.add_class(parse_class(++i).value);
      }

      default: {
        std::cout << "[ ERROR ] expected class declaration, but was "
                  << token_id_to_string(tok->get_token_id()) << std::endl;
      }
    }
  }

  return {nullptr};
}
}  // namespace

inline ast make_ast(token_vector& tokens) {
  std::erase_if(
      tokens, [](auto& tok) { return tok->get_token_id() == token_id::Space; });

  ast_parser parser(tokens);

  std::cout << "after erased" << std::endl;

  for (auto& tok : tokens) {
    tok->print();
  }

  return ast(parser.parse_program().value);
}
}  // namespace tree

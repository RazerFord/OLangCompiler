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

template <class T>
struct result<std::shared_ptr<T>> {
  std::shared_ptr<T> value;

  operator bool() const noexcept { return value != nullptr; }
};

class ast_parser {
 private:
  const token_vector& tokens_;

  inline result<std::shared_ptr<body_node>> parse_body(std::size_t&);

  inline result<std::shared_ptr<parameters_node>> parse_parameters(
      std::size_t&);

  inline result<std::shared_ptr<parameter_node>> parse_parameter(std::size_t&);

  inline result<std::shared_ptr<expression_node>> parse_expression(
      std::size_t&);

  inline result<std::shared_ptr<variable_node>> parse_variable(std::size_t&);

  inline result<std::shared_ptr<method_node>> parse_method(std::size_t&);

  inline result<std::shared_ptr<constructor_node>> parse_constructor(
      std::size_t&);

  inline result<std::shared_ptr<member_node>> parse_member(std::size_t&);

  inline result<std::shared_ptr<return_statement_node>> parse_return(
      std::size_t& first_token);

  inline result<std::shared_ptr<primary_node>> parse_primary(
      std::size_t& first_token);

  inline result<std::shared_ptr<class_name_node>> parse_generic(
      std::size_t& first_token);

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

  inline bool skip_newline(size_t& token_i) {
    for (; token_i < tokens_.size(); ++token_i) {
      if (tokens_.at(token_i)->get_token_id() != token_id::NewLine) {
        --token_i;
        break;
      }
    }
    return true;
  }
};

inline result<std::shared_ptr<identifier_node>> ast_parser::parse_identifier(
    std::size_t& first_token) {
  auto id_node = std::make_shared<identifier_node>();

  auto& tok = tokens_.at(first_token);

  using ptr_tok_id = token::identifier*;
  ptr_tok_id id = nullptr;

  if ((tok->get_token_id() == token_id::Identifier) &&
      (id = dynamic_cast<ptr_tok_id>(tok.get()))) {
    id_node->set_name(id->get_value());
    std::cout << "[ INFO ] class name identifier: " << id->get_value()
              << std::endl;
    return {id_node};
  } else {
    std::cout << "[ ERROR ] expected identifier of class, but was: "
              << token_id_to_string(tok->get_token_id()) << std::endl;
    return {nullptr};
  }
}

inline result<std::shared_ptr<parameter_node>> ast_parser::parse_parameter(
    size_t& first_token) {
  result<std::shared_ptr<class_name_node>> class_name;
  if (auto identifier = parse_identifier(first_token);
      identifier &&
      tokens_.at(++first_token)->get_token_id() == token_id::Colon &&
      (class_name = parse_class_name(++first_token))) {
    auto parameter = std::make_shared<parameter_node>();
    parameter->set_identifier(identifier.value);
    parameter->set_class_name(class_name.value);
    return {parameter};
  }
  return {nullptr};
}

inline result<std::shared_ptr<parameters_node>> ast_parser::parse_parameters(
    size_t& first_token) {
  if (tokens_.at(first_token)->get_token_id() == token_id::LBracket) {
    std::shared_ptr<parameters_node> parameters;
    while (true) {
      if (tokens_.at(++first_token)->get_token_id() == token_id::RBracket)
        break;

      if (auto tmp_parameter = parse_parameter(first_token); tmp_parameter) {
        parameters->add_parameter(tmp_parameter.value);
      }

      auto cur_token = tokens_.at(++first_token)->get_token_id();
      if (cur_token != token_id::Comma && cur_token == token_id::RBracket) {
        break;
      }

      if (cur_token != token_id::Comma) {
        return {nullptr};
      }
    }
    return {parameters};
  }

  return {nullptr};
}

inline result<std::shared_ptr<constructor_node>> ast_parser::parse_constructor(
    size_t& first_token) {
  result<std::shared_ptr<body_node>> body;
  if (auto parameters = parse_parameters(first_token);
      parameters && tokens_.at(++first_token)->get_token_id() == token_id::Is &&
      (body = parse_body(++first_token)) &&
      tokens_.at(++first_token)->get_token_id() == token_id::End) {
    auto constructor = std::make_shared<constructor_node>();
    constructor->set_parameters(parameters.value);
    constructor->set_body(body.value);
    return {constructor};
  }
  return {nullptr};
}

inline result<std::shared_ptr<method_node>> ast_parser::parse_method(
    std::size_t& first_token) {
  result<std::shared_ptr<parameters_node>> parameters;
  if (auto identifier = parse_identifier(first_token);
      identifier && (parameters = parse_parameters(++first_token))) {
    auto method = std::make_shared<method_node>();
    result<std::shared_ptr<identifier_node>> return_type;
    if (tokens_.at(++first_token)->get_token_id() == token_id::Colon &&
        (return_type = parse_identifier(++first_token))) {
      method->set_return_type(return_type.value);
    }

    result<std::shared_ptr<body_node>> body;
    if (tokens_.at(++first_token)->get_token_id() == token_id::Is &&
        (body = parse_body(++first_token)) &&
        tokens_.at(++first_token)->get_token_id() == token_id::End) {
      method->set_identifier(identifier.value);
      method->set_parameters(parameters.value);
      method->set_body(body.value);
    }
  }
  return {nullptr};
}

inline result<std::shared_ptr<variable_node>> ast_parser::parse_variable(
    std::size_t& first_token) {
  if (auto identifier = parse_identifier(first_token);
      identifier &&
      tokens_.at(++first_token)->get_token_id() == token_id::Colon) {
    if (auto expression = parse_expression(++first_token); expression) {
      auto variable = std::make_shared<variable_node>();
      variable->set_identifier(identifier.value);
      variable->set_expression(expression.value);
      return {variable};
    }
  }
  return {nullptr};
}
inline result<std::shared_ptr<member_node>> ast_parser::parse_member(
    std::size_t& first_token) {
  for (; first_token < tokens_.size(); ++first_token) {
    switch (tokens_.at(first_token)->get_token_id()) {
      case token_id::Var: {
        return {parse_variable(++first_token).value};
      }
      case token_id::Method: {
        return {parse_method(++first_token).value};
      }
      case token_id::This: {
        return {parse_constructor(++first_token).value};
      }
      default: {
        break;
      }
    }
  }
  return {nullptr};
}
inline result<std::shared_ptr<primary_node>> ast_parser::parse_primary(
    std::size_t& first_token) {
  auto& tok = tokens_[first_token];
  switch (tok->get_token_id()) {
    case token_id::RealLiteral: {
      break;
    }
    case token_id::BooleanLiteral: {
      break;
    }
    case token_id::This: {
      break;
    }
    case token_id::Class: {
      break;
    }
    default: {
      std::cout << "[ ERROR ] expected identifier of class, but was: "
                << token_id_to_string(tok->get_token_id()) << std::endl;
    }
  }
  return {};
}

inline result<std::shared_ptr<expression_node>> ast_parser::parse_expression(
    std::size_t& first_token) {
  auto expression = std::make_shared<expression_node>();
  expression->set_primary(parse_primary(first_token).value);
  return {};
}

inline result<std::shared_ptr<return_statement_node>> ast_parser::parse_return(
    std::size_t& first_token) {
  auto return_node = std::make_shared<return_statement_node>();
  return_node->set_expression(parse_expression(first_token).value);
  return {return_node};
}

inline result<std::shared_ptr<body_node>> ast_parser::parse_body(
    std::size_t& first_token) {
  auto body = std::make_shared<body_node>();
  for (; first_token < tokens_.size(); first_token++) {
    auto& tok = tokens_[first_token];
    switch (tok->get_token_id()) {
      case token_id::Assign: {
        //
      }

      case token_id::While: {
        //
      }

      case token_id::If: {
        //
      }

      case token_id::Return: {
        body->add_statement(parse_return(first_token).value);
      }

      case token_id::Var: {
        // TODO: parse variable by @Vlad
      }

      default: {
        std::cout << "[ ERROR ] expected assignment, while, if, return "
                     "statement or variable declaration, but was: "
                  << token_id_to_string(tok->get_token_id()) << std::endl;
      }
    }
  }
  return {};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_generic(
    std::size_t& first_token) {
  auto& lbracket = tokens_.at(first_token);
  result<std::shared_ptr<identifier_node>> result;
  auto class_name = std::make_shared<class_name_node>();
  const std::unique_ptr<token::token>* bracket = nullptr;

  if (lbracket->get_token_id() == token_id::LSBracket &&
      (result = parse_identifier(++first_token)) &&
      (bracket = &tokens_.at(++first_token))) {
    class_name->set_identifier(result.value);

    if ((*bracket)->get_token_id() == token_id::LSBracket) {
      class_name->set_generic(parse_generic(first_token).value);
      bracket = &tokens_.at(++first_token);
    }

    if ((*bracket)->get_token_id() == token_id::RSBracket) {
      std::cout << "[ INFO ] generic parsed" << std::endl;

      return {class_name};
    } else {
      std::cout << "[ ERROR ] expected ']', but was "
                << token_id_to_string(token_id::RSBracket) << std::endl;

      return {nullptr};
    }
  } else {
    return {nullptr};
  }
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_class_name(
    std::size_t& first_token) {
  auto class_name = std::make_shared<class_name_node>();

  class_name->set_identifier(parse_identifier(first_token).value);
  first_token++;
  class_name->set_generic(parse_generic(first_token).value);

  return {class_name};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_extends(
    std::size_t& first_token) {
  auto class_name = std::make_shared<class_name_node>();

  auto& tok = tokens_.at(first_token);
  if (tok->get_token_id() == token_id::Extends) {
    class_name->set_generic(parse_class_name(++first_token).value);
    return {class_name};
  } else if (tok->get_token_id() == token_id::Is) {
    return {class_name};
  } else {
    std::cout << "[ ERROR ] expected 'extends' or 'is', but was: "
              << token_id_to_string(tok->get_token_id()) << std::endl;
    return {nullptr};
  }
}

inline result<std::shared_ptr<class_node>> ast_parser::parse_class(
    std::size_t& first_token) {
  auto node = std::make_shared<class_node>();

  node->set_class_name(parse_class_name(first_token).value);
  node->set_extends(parse_extends(++first_token).value);
  first_token++;
  auto& tok = tokens_[first_token];
  if (tok->get_token_id() == token_id::Is) {
    // parse members decl
    if (tok->get_token_id() == token_id::End) {
    } else {
    }
  } else {
    std::cout << "[ ERROR ] expected 'is' but was: "
              << token_id_to_string(tok->get_token_id()) << std::endl;
    return {nullptr};
  }

  return {node};
}

inline result<std::shared_ptr<program_node>> ast_parser::parse_program(
    const std::size_t& first_token) {
  program_node program;

  for (std::size_t i = first_token; i < tokens_.size(); i++) {
    auto& tok = tokens_[i];
    switch (tok->get_token_id()) {
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
  std::erase_if(tokens, [](auto& tok) {
    auto id = tok->get_token_id();
    return id == token_id::Space || id == token_id::NewLine;
  });

  ast_parser parser(tokens);

  std::cout << "after erased" << std::endl;

  for (auto& tok : tokens) {
    tok->print();
  }

  return ast(parser.parse_program().value);
}
}  // namespace tree

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
  bool ok = false;

  explicit operator bool() const noexcept { return value != nullptr || ok; }
};

class ast_parser {
 private:
  const token_vector& tokens_;

  inline result<std::shared_ptr<statement_node>> parse_if(std::size_t&);

  inline result<std::shared_ptr<statement_node>> parse_while(std::size_t&);

  inline result<std::shared_ptr<statement_node>> parse_assign(std::size_t&);

  inline result<std::shared_ptr<arguments_node>> parse_arguments(std::size_t&);

  inline result<std::shared_ptr<body_node>> parse_body(std::size_t&);

  inline result<std::shared_ptr<body_node>> parse_scope(std::size_t&);

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

inline result<std::shared_ptr<statement_node>> ast_parser::parse_if(
    std::size_t& first_token) {
  if (auto tok_id = tokens_[first_token]->get_token_id();
      tok_id != token_id::If) {
    std::cout << "[ ERROR ] expected If, but was " << token_id_to_string(tok_id)
              << std::endl;
    return {};
  }
  auto statement = std::make_shared<if_statement_node>();

  auto expr = parse_expression(++first_token);
  statement->set_expression(expr.value);
  if (auto tok_id = tokens_[first_token++]->get_token_id();
      tok_id != token_id::Then || !expr) {
    std::cout << "[ ERROR ] expected Then for If, but was "
              << token_id_to_string(tok_id) << std::endl;
    return {};
  }

  auto true_branch = parse_expression(++first_token);
  statement->set_expression(true_branch.value);

  if (auto tok_id = tokens_[first_token]->get_token_id();
      tok_id != token_id::Else || !true_branch) {
    std::cout << "[ ERROR ] expected True Branch, but was "
              << token_id_to_string(tok_id) << std::endl;
    return {};
  }

  auto false_branch = parse_expression(++first_token);
  statement->set_expression(false_branch.value);

  return {statement, false_branch.ok};
}

inline result<std::shared_ptr<statement_node>> ast_parser::parse_while(
    std::size_t& first_token) {
  if (auto tok_id = tokens_[first_token]->get_token_id();
      tok_id != token_id::While) {
    std::cout << "[ ERROR ] expected Identifier for assignment, but was "
              << token_id_to_string(tok_id) << std::endl;
    return {};
  }
  auto statement = std::make_shared<while_loop_node>();

  auto expr = parse_expression(++first_token);
  statement->set_expression(expr.value);

  if (auto tok_id = tokens_[first_token++]->get_token_id();
      tok_id != token_id::Loop || !expr) {
    std::cout << "[ ERROR ] expected Loop for assignment, but was "
              << token_id_to_string(tok_id) << std::endl;
    return {};
  }
  auto scope = parse_scope(first_token);
  statement->set_body_node(parse_scope(first_token).value);

  return {statement, scope.ok};
}

inline result<std::shared_ptr<statement_node>> ast_parser::parse_assign(
    std::size_t& first_token) {
  auto& identifier = tokens_[first_token];
  if (identifier->get_token_id() != token_id::Identifier) {
    std::cout << "[ ERROR ] expected Identifier for assignment, but was "
              << token_id_to_string(identifier->get_token_id()) << std::endl;
    return {};
  }
  auto statement = std::make_shared<assignment_node>();
  statement->set_identifier(parse_identifier(first_token).value);

  if (tokens_[++first_token]->get_token_id() != token_id::Assign) {
    std::cout << "[ ERROR ] expected Assign for assignment, but was "
              << token_id_to_string(tokens_[first_token - 1]->get_token_id())
              << std::endl;
    return {};
  }
  auto expr = parse_expression(first_token);
  statement->set_expression(expr.value);
  return {statement, expr.ok};
}

inline result<std::shared_ptr<identifier_node>> ast_parser::parse_identifier(
    std::size_t& first_token) {
  auto id_node = std::make_shared<identifier_node>();

  auto& tok = tokens_.at(first_token);

  using ptr_tok_id = token::identifier*;
  ptr_tok_id id = nullptr;

  if ((tok->get_token_id() == token_id::Identifier) &&
      (id = dynamic_cast<ptr_tok_id>(tok.get()))) {
    id_node->set_name(id->get_value());
    std::cout << "[ INFO ] identifier: " << id->get_value() << std::endl;
    return {id_node};
  } else {
    std::cout << "[ ERROR ] expected identifier, but was: "
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
    auto parameters = std::make_shared<parameters_node>();
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
    return {parameters, true};
  }

  return {nullptr};
}

inline result<std::shared_ptr<constructor_node>> ast_parser::parse_constructor(
    size_t& first_token) {
  result<std::shared_ptr<body_node>> body;
  if (auto parameters = parse_parameters(first_token);
      parameters && tokens_.at(++first_token)->get_token_id() == token_id::Is &&
      (body = parse_body(first_token)) &&
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
    if (tokens_.at(++first_token)->get_token_id() == token_id::Colon) {
      method->set_return_type(parse_identifier(++first_token).value);
    } else {
      --first_token;
    }

    result<std::shared_ptr<body_node>> body;
    if (tokens_.at(++first_token)->get_token_id() == token_id::Is &&
        (body = parse_body(++first_token)) &&
        tokens_.at(++first_token)->get_token_id() == token_id::End) {
      method->set_identifier(identifier.value);
      method->set_parameters(parameters.value);
      method->set_body(body.value);
    }
    return {method};
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
    auto tok_id = tokens_.at(first_token)->get_token_id();
    switch (tok_id) {
      case token_id::Var: {
        return {parse_variable(++first_token).value};
      }
      case token_id::Method: {
        return {parse_method(++first_token).value};
      }
      case token_id::This: {
        return {parse_constructor(++first_token).value};
      }
      case token_id::End: {
        return {};
      }
      default: {
        std::cout << "[ ERROR ] expected member, but was "
                  << token_id_to_string(tok_id) << std::endl;
      }
    }
  }
  return {};
}

inline result<std::shared_ptr<primary_node>> ast_parser::parse_primary(
    std::size_t& first_token) {
  auto& tok = tokens_[first_token];
  switch (tok->get_token_id()) {
    case token_id::IntegerLiteral: {
      auto literal_token =
          dynamic_cast<token::basic_template_token<int32_t>*>(tok.get());
      return {
          std::make_shared<literal_node<int32_t>>(literal_token->get_value())};
    }
    case token_id::RealLiteral: {
      auto literal_token =
          dynamic_cast<token::basic_template_token<double_t>*>(tok.get());
      return {
          std::make_shared<literal_node<double_t>>(literal_token->get_value())};
    }
    case token_id::BooleanLiteral: {
      auto literal_token =
          dynamic_cast<token::basic_template_token<bool>*>(tok.get());
      return {std::make_shared<literal_node<bool>>(literal_token->get_value())};
    }
    case token_id::This: {
      return {std::make_shared<this_node>()};
    }
    case token_id::Null: {
      return {std::make_shared<null_node>()};
    }
    case token_id::Identifier: {
      return {parse_class_name(first_token).value};
    }
    default: {
      std::cout << "[ ERROR ] expected identifier of class, but was: "
                << token_id_to_string(tok->get_token_id()) << std::endl;
    }
  }
  return {};
}

inline result<std::shared_ptr<arguments_node>> ast_parser::parse_arguments(
    std::size_t& first_token) {
  if (tokens_.at(first_token)->get_token_id() == token_id::LBracket) {
    auto parameters = std::make_shared<arguments_node>();
    while (true) {
      if (tokens_.at(++first_token)->get_token_id() == token_id::RBracket)
        break;

      if (auto tmp_expression = parse_expression(first_token); tmp_expression) {
        parameters->add_expression(tmp_expression.value);
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

inline result<std::shared_ptr<expression_node>> ast_parser::parse_expression(
    std::size_t& first_token) {
  auto expression = std::make_shared<expression_node>();
  expression->set_primary(parse_primary(first_token).value);
  if (tokens_.at(++first_token)->get_token_id() == token_id::Dot) {
    expression->set_identifier(parse_identifier(++first_token).value);
    expression->set_arguments(parse_arguments(++first_token).value);
  } else {
    --first_token;
  }
  return {expression};
}

inline result<std::shared_ptr<return_statement_node>> ast_parser::parse_return(
    std::size_t& first_token) {
  auto return_node = std::make_shared<return_statement_node>();
  return_node->set_expression(parse_expression(first_token).value);
  return {return_node};
}

inline result<std::shared_ptr<body_node>> ast_parser::parse_body(
    std::size_t& first_token) {
  if (auto tok = tokens_[first_token]->get_token_id(); tok != token_id::Is) {
    std::cout << "[ ERROR ] expected Is, but was: " << token_id_to_string(tok)
              << std::endl;
    return {};
  }

  return parse_scope(first_token);
}

inline result<std::shared_ptr<body_node>> ast_parser::parse_scope(
    std::size_t& first_token) {
  auto body = std::make_shared<body_node>();

  for (first_token++; first_token < tokens_.size(); first_token++) {
    auto& tok = tokens_[first_token];
    switch (tok->get_token_id()) {
      case token_id::Assign: {
        first_token--;  // to identifier
        body->add_node(parse_assign(first_token).value);
        continue;
      }

      case token_id::While: {
        body->add_node(parse_while(first_token).value);
        continue;
      }

      case token_id::If: {
        body->add_node(parse_if(first_token).value);
        continue;
      }

      case token_id::Return: {
        body->add_node(parse_return(first_token).value);
        continue;
      }

      case token_id::Var: {
        body->add_node(parse_variable(++first_token).value);
        continue;
      }

      case token_id::End: {
        return {body};
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
  first_token++;
  auto& lbracket = tokens_.at(first_token);
  result<std::shared_ptr<identifier_node>> result;
  auto class_name = std::make_shared<class_name_node>();
  token_id bracket_id;

  if (lbracket->get_token_id() == token_id::LSBracket &&
      (result = parse_identifier(++first_token))) {
    class_name->set_identifier(result.value);
    bracket_id = tokens_.at(++first_token)->get_token_id();

    if (bracket_id == token_id::LSBracket) {
      class_name->set_generic(parse_generic(first_token).value);
      bracket_id = tokens_.at(first_token)->get_token_id();
    }

    if (bracket_id == token_id::RSBracket) {
      std::cout << "[ INFO ] generic parsed" << std::endl;
      return {class_name};
    } else {
      std::cout << "[ ERROR ] expected ']', but was "
                << token_id_to_string(token_id::RSBracket) << std::endl;

      return {nullptr};
    }
  } else {
    first_token--;
    return {nullptr};
  }
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_class_name(
    std::size_t& first_token) {
  auto class_name = std::make_shared<class_name_node>();

  class_name->set_identifier(parse_identifier(first_token).value);
  class_name->set_generic(parse_generic(first_token).value);

  return {class_name};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_extends(
    std::size_t& first_token) {
  first_token++;
  auto tok_id = tokens_.at(first_token)->get_token_id();
  if (tok_id == token_id::Extends) {
    return parse_class_name(first_token);
  } else if (tok_id == token_id::Is) {
    return {nullptr, true};
  } else {
    std::cout << "[ ERROR ] expected 'extends' or 'is', but was: "
              << token_id_to_string(tok_id) << std::endl;
    first_token--;
    return {nullptr};
  }
}

inline result<std::shared_ptr<class_node>> ast_parser::parse_class(
    std::size_t& first_token) {
  first_token++;
  auto node = std::make_shared<class_node>();

  node->set_class_name(parse_class_name(first_token).value);
  node->set_extends(parse_extends(first_token).value);

  auto& tok = tokens_[first_token];
  if (tok->get_token_id() != token_id::Is) {
    std::cout << "[ ERROR ] expected 'is' but was: "
              << token_id_to_string(tok->get_token_id()) << std::endl;
    return {nullptr};
  }

  for (std::size_t& i = ++first_token;
       i < tokens_.size() && tokens_[i]->get_token_id() != token_id::End; i++) {
    node->add_member(parse_member(i).value);
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
        program.add_class(parse_class(i).value);
        break;
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

  return {parser.parse_program().value};
}
}  // namespace tree

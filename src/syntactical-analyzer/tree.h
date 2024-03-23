#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <vector>

#include "./../lexical-analyzer/token-code.h"
#include "./../lexical-analyzer/token.h"
#include "./../logging/logger.h"
#include "details.h"
#include "syntactical-analyzer/token_stream.h"

namespace tree {
using token_vector = std::vector<std::unique_ptr<token::token>>;
using namespace details;

class ast {
 private:
  std::shared_ptr<program_node> root_;

 public:
  ast(const std::shared_ptr<program_node>& root) : root_{root} {}

  void print() const noexcept { root_->print(); }
};

namespace {
constexpr auto tok_to_str = token_id_to_string;

void log_expected_actual(const token_id& e, const token_id& a) {
  logger::info("expected", tok_to_str(e), ", but", "actual", tok_to_str(a));
}

void log_error_lang(const span& span, const token_id& e, const token_id& a) {
  std::string exp = logger::tolower(logger::trim(tok_to_str(e)));
  std::string act = logger::tolower(logger::trim(tok_to_str(a)));

  std::cout << span.line_ << ":" << span.offset_ << " expected \"" << exp
            << "\", but was \"" << act << "\"" << std::endl;
}

void log_error_lang(const span& span, const std::string& msg) {
  std::cout << span.line_ << ":" << span.offset_ << " " << msg << std::endl;
}

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
  token_stream stream_;

  inline result<std::shared_ptr<statement_node>> parse_if();

  inline result<std::shared_ptr<statement_node>> parse_while();

  inline result<std::shared_ptr<statement_node>> parse_assign();

  inline result<std::shared_ptr<arguments_node>> parse_arguments();

  inline result<std::shared_ptr<body_node>> parse_body();

  inline result<std::shared_ptr<body_node>> parse_scope();

  inline result<std::shared_ptr<parameters_node>> parse_parameters();

  inline result<std::shared_ptr<parameter_node>> parse_parameter();

  inline result<std::shared_ptr<expression_node>> parse_expression();

  inline result<std::shared_ptr<variable_node>> parse_variable();

  inline result<std::shared_ptr<method_node>> parse_method();

  inline result<std::shared_ptr<constructor_node>> parse_constructor();

  inline result<std::shared_ptr<member_node>> parse_member();

  inline result<std::shared_ptr<return_statement_node>> parse_return();

  inline result<std::shared_ptr<primary_node>> parse_primary();

  inline result<std::shared_ptr<class_name_node>> parse_generic();

  inline result<std::shared_ptr<identifier_node>> parse_identifier();

  inline result<std::shared_ptr<class_name_node>> parse_class_name();

  inline result<std::shared_ptr<class_name_node>> parse_extends();

  inline result<std::shared_ptr<class_node>> parse_class();

  inline result<std::shared_ptr<primary_node>> parse_literal();

  inline result<std::shared_ptr<primary_node>> parse_keyword();

 public:
  ast_parser(const token_vector& token) : stream_{token} {}
  inline result<std::shared_ptr<program_node>> parse_program();
};

inline result<std::shared_ptr<statement_node>> ast_parser::parse_if() {
  if (auto tok_id = stream_.get_token_id(); tok_id != token_id::If) {
    log_expected_actual(token_id::If, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::If, tok_id);
    return {};
  }
  auto statement = std::make_shared<if_statement_node>();

  auto expr = parse_expression();
  statement->set_expression(expr.value);
  if (auto tok_id = stream_.next_and_token_id();
      tok_id != token_id::Then || !expr) {
    log_expected_actual(token_id::Then, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::Then, tok_id);
    return {};
  }

  auto true_branch = parse_scope();
  statement->set_true_body(true_branch.value);

  if (auto tok_id = stream_.get_token_id();
      (tok_id != token_id::Else && tok_id != token_id::End) || !true_branch) {
    log_expected_actual(token_id::Else, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::Else, tok_id);
    return {};
  }

  result<std::shared_ptr<body_node>> false_branch;
  if (stream_.get_token_id() == token_id::Else) {
    false_branch = parse_scope();
    statement->set_false_body(false_branch.value);
  }

  logger::info("if parsed");

  return {statement, false_branch.ok};
}

inline result<std::shared_ptr<statement_node>> ast_parser::parse_while() {
  if (auto tok_id = stream_.get_token_id(); tok_id != token_id::While) {
    log_expected_actual(token_id::While, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::While, tok_id);
    return {};
  }
  auto statement = std::make_shared<while_loop_node>();

  auto expr = parse_expression();
  statement->set_expression(expr.value);

  if (auto tok_id = stream_.next_and_token_id();
      tok_id != token_id::Loop || !expr) {
    log_expected_actual(token_id::Loop, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::Loop, tok_id);
    return {};
  }
  auto scope = parse_scope();
  statement->set_body_node(scope.value);

  logger::info("while loop parsed");

  return {statement, scope.ok};
}

inline result<std::shared_ptr<identifier_node>> ast_parser::parse_identifier() {
  auto id_node = std::make_shared<identifier_node>();

  using ptr_tok_id = token::identifier*;
  ptr_tok_id id = nullptr;

  if ((stream_.next_and_token_id() == token_id::Identifier) &&
      (id = dynamic_cast<ptr_tok_id>(stream_.token().get()))) {
    id_node->set_name(id->get_value());
    logger::info("identifier", id->get_value(), "parsed");
    return {id_node};
  } else {
    log_expected_actual(token_id::Identifier, stream_.get_token_id());
    log_error_lang(stream_.token()->get_span(), token_id::Identifier,
                   stream_.get_token_id());
    return {nullptr};
  }
}

inline result<std::shared_ptr<parameter_node>> ast_parser::parse_parameter() {
  result<std::shared_ptr<class_name_node>> class_name;
  if (auto identifier = parse_identifier();
      identifier && stream_.next_and_token_id() == token_id::Colon &&
      (class_name = parse_class_name())) {
    auto parameter = std::make_shared<parameter_node>();
    parameter->set_identifier(identifier.value);
    parameter->set_class_name(class_name.value);

    logger::info("parameter parsed");

    return {parameter};
  }
  logger::error("parameter parsing error");
  log_error_lang(stream_.token()->get_span(), token_id::Colon,
                 stream_.get_token_id());
  return {nullptr};
}

inline result<std::shared_ptr<parameters_node>> ast_parser::parse_parameters() {
  if (stream_.next_token_id() == token_id::LBracket) {
    ++stream_;
    auto parameters = std::make_shared<parameters_node>();
    while (true) {
      if (stream_.next_token_id() == token_id::RBracket) break;

      if (auto tmp_parameter = parse_parameter(); tmp_parameter) {
        parameters->add_parameter(tmp_parameter.value);
      }

      auto cur_token = stream_.next_token_id();
      if (cur_token != token_id::Comma && cur_token == token_id::RBracket) {
        break;
      }
      ++stream_;
      if (cur_token != token_id::Comma) {
        return {nullptr};
      }
    }
    ++stream_;
    logger::info("parameters parsed");

    return {parameters, true};
  }
  logger::error("parameters parsing error");
  log_error_lang(stream_.token()->get_span(), token_id::LBracket,
                 stream_.next_token_id());

  return {nullptr};
}

inline result<std::shared_ptr<constructor_node>>
ast_parser::parse_constructor() {
  result<std::shared_ptr<body_node>> body;
  if (auto parameters = parse_parameters();
      parameters && stream_.next_and_token_id() == token_id::Is &&
      (body = parse_body()) && stream_.get_token_id() == token_id::End) {
    auto constructor = std::make_shared<constructor_node>();
    constructor->set_parameters(parameters.value);
    constructor->set_body(body.value);

    logger::info("constructor parsed");

    return {constructor};
  }
  logger::error("constructor parsing error");
  if (stream_.get_token_id() == token_id::Is) {
    log_error_lang(stream_.token()->get_span(), token_id::Is,
                   stream_.get_token_id());
  }
  if (stream_.next_and_token_id() == token_id::End) {
    log_error_lang(stream_.token()->get_span(), token_id::End,
                   stream_.get_token_id());
  }

  return {nullptr};
}

inline result<std::shared_ptr<method_node>> ast_parser::parse_method() {
  result<std::shared_ptr<parameters_node>> parameters;
  if (auto identifier = parse_identifier();
      identifier && (parameters = parse_parameters())) {
    auto method = std::make_shared<method_node>();
    if (stream_.next_token_id() == token_id::Colon) {
      ++stream_;
      method->set_return_type(parse_identifier().value);
    }

    result<std::shared_ptr<body_node>> body;
    if (stream_.next_and_token_id() == token_id::Is && (body = parse_body()) &&
        stream_.get_token_id() == token_id::End) {
      method->set_identifier(identifier.value);
      method->set_parameters(parameters.value);
      method->set_body(body.value);
    }
    logger::info("method parsed");

    return {method};
  }
  logger::error("method parsing error");

  return {nullptr};
}

inline result<std::shared_ptr<variable_node>> ast_parser::parse_variable() {
  if (auto identifier = parse_identifier();
      identifier && stream_.next_and_token_id() == token_id::Colon) {
    if (auto expression = parse_expression(); expression) {
      auto variable = std::make_shared<variable_node>();
      variable->set_identifier(identifier.value);
      variable->set_expression(expression.value);

      logger::info("variable parsed");

      return {variable};
    }
    log_error_lang(stream_.next_token()->get_span(), token_id::Colon,
                   stream_.next_token_id());
  } else {
    logger::error("variable parsing error", tok_to_str(stream_.get_token_id()));
    log_error_lang(stream_.token()->get_span(), token_id::Colon,
                   stream_.get_token_id());
  }
  return {nullptr};
}

inline result<std::shared_ptr<member_node>> ast_parser::parse_member() {
  for (; stream_; ++stream_) {
    switch (stream_.get_token_id()) {
      case token_id::Var: {
        return {parse_variable().value};
      }
      case token_id::Method: {
        return {parse_method().value};
      }
      case token_id::This: {
        return {parse_constructor().value};
      }
      case token_id::End: {
        return {};
      }
      default: {
        logger::error("raw token", tok_to_str(stream_.get_token_id()));
        log_error_lang(stream_.token()->get_span(),
                       " class member parsing error");
      }
    }
  }
  return {};
}

inline result<std::shared_ptr<primary_node>> ast_parser::parse_literal() {
  auto&& tok = stream_.next_and_token();
  switch (stream_.get_token_id()) {
    case token_id::IntegerLiteral: {
      token::basic_template_token<int32_t>* literal_token =
          dynamic_cast<token::basic_template_token<int32_t>*>(tok.get());
      return {std::make_shared<literal_node<int32_t>>(*literal_token)};
    }
    case token_id::RealLiteral: {
      token::basic_template_token<double_t>* literal_token =
          dynamic_cast<token::basic_template_token<double_t>*>(tok.get());
      return {std::make_shared<literal_node<double_t>>(*literal_token)};
    }
    case token_id::BooleanLiteral: {
      token::basic_template_token<bool>* literal_token =
          dynamic_cast<token::basic_template_token<bool>*>(tok.get());
      return {std::make_shared<literal_node<bool>>(*literal_token)};
    }
    default:
      logger::error("raw token");
  }
  return {};
}

inline result<std::shared_ptr<primary_node>> ast_parser::parse_keyword() {
  const auto& tok = stream_.next_and_token();
  switch (tok->get_token_id()) {
    case token_id::This: {
      auto this_token = dynamic_cast<token::identifier*>(tok.get());
      return {std::make_shared<this_node>(*this_token)};
    }
    case token_id::Null: {
      auto null_token = dynamic_cast<token::identifier*>(tok.get());
      return {std::make_shared<null_node>(*null_token)};
    }
    case token_id::Base: {
      auto base = std::make_shared<base_node>();
      base->set_arguments(parse_arguments().value);
      return {base};
    }
    default:
      logger::error("raw token", tok_to_str(stream_.get_token_id()));
  }
  return {};
}

inline result<std::shared_ptr<primary_node>> ast_parser::parse_primary() {
  switch (stream_.next_token_id()) {
    case token_id::IntegerLiteral:
    case token_id::RealLiteral:
    case token_id::BooleanLiteral: {
      logger::info("literal:", tok_to_str(stream_.next_token_id()));
      return parse_literal();
    }
    case token_id::Base:
    case token_id::This:
    case token_id::Null: {
      return parse_keyword();
    }
    case token_id::Identifier: {
      return {parse_class_name().value};
    }
    default: {
      logger::error("raw token", tok_to_str(stream_.get_token_id()));
    }
  }
  return {};
}

inline result<std::shared_ptr<arguments_node>> ast_parser::parse_arguments() {
  if (stream_.next_token_id() == token_id::LBracket) {
    ++stream_;
    auto arguments = std::make_shared<arguments_node>();
    while (true) {
      if (stream_.next_token_id() == token_id::RBracket) break;

      if (auto tmp_expression = parse_expression(); tmp_expression) {
        arguments->add_expression(tmp_expression.value);
      }

      auto cur_token = stream_.next_token_id();
      if (cur_token != token_id::Comma && cur_token == token_id::RBracket) {
        break;
      }
      ++stream_;

      if (cur_token != token_id::Comma) {
        return {nullptr};
      }
    }
    ++stream_;

    return {arguments};
  }

  return {nullptr};
}

inline result<std::shared_ptr<expression_node>> ast_parser::parse_expression() {
  auto expression = std::make_shared<expression_node>();
  if (auto primary = parse_primary(); primary) {
    expression->set_primary(primary.value);
    if (stream_.next_token_id() == token_id::Dot) {
      while (stream_.next_token_id() == token_id::Dot) {
        ++stream_;
        expression->add_value({parse_identifier().value, parse_arguments().value});
      }
    } else if (auto arguments = parse_arguments(); arguments) {
      expression->add_value({nullptr, arguments.value});
    }
    logger::info("arguments parsed");
    return {expression};
  } else {
    log_error_lang(stream_.token()->get_span(), "expression parsing error");
  }
  return {};
}

inline result<std::shared_ptr<return_statement_node>>
ast_parser::parse_return() {
  auto return_node = std::make_shared<return_statement_node>();
  if (stream_.next_token_id() != token_id::NewLine)
    return_node->set_expression(parse_expression().value);
  else {
    ++stream_;
  }
  return {return_node};
}

inline result<std::shared_ptr<body_node>> ast_parser::parse_body() {
  if (auto tok_id = stream_.get_token_id(); tok_id != token_id::Is) {
    log_expected_actual(token_id::Is, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::Is, tok_id);
    return {};
  }

  return parse_scope();
}

inline result<std::shared_ptr<body_node>> ast_parser::parse_scope() {
  auto body = std::make_shared<body_node>();

  for (++stream_; stream_; ++stream_) {
    auto& tok = stream_.token();
    switch (tok->get_token_id()) {
      case token_id::While: {
        body->add_node(parse_while().value);
        continue;
      }

      case token_id::If: {
        body->add_node(parse_if().value);
        continue;
      }

      case token_id::Return: {
        body->add_node(parse_return().value);
        continue;
      }

      case token_id::Var: {
        body->add_node(parse_variable().value);
        continue;
      }
      case token_id::Else:
      case token_id::End: {
        return {body};
      }

      case token_id::Base:
      case token_id::This:
      case token_id::Identifier: {
        --stream_;
        auto lexpression = parse_expression();
        if (stream_.next_token_id() == token_id::Assign) {
          ++stream_;
          auto rexpression = parse_expression();
          auto assign = std::make_shared<assignment_node>();
          assign->set_lexpression(lexpression.value);
          assign->set_rexpression(rexpression.value);
          body->add_node(assign);
        } else {
          body->add_node(lexpression.value);
        }

        continue;
      }

      default: {
        logger::error("raw token", tok_to_str(tok->get_token_id()));
      }
    }
  }
  return {};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_generic() {
  result<std::shared_ptr<identifier_node>> result;
  auto class_name = std::make_shared<class_name_node>();
  token_id bracket_id;

  if (stream_.next_token_id() == token_id::LSBracket && ++stream_ &&
      (result = parse_identifier())) {
    class_name->set_identifier(result.value);
    bracket_id = stream_.next_and_token_id();

    if (bracket_id == token_id::LSBracket) {
      class_name->set_generic(parse_generic().value);
      bracket_id = stream_.get_token_id();
    }

    if (bracket_id == token_id::RSBracket) {
      logger::info("generic parsed");
      return {class_name};
    } else {
      log_expected_actual(token_id::RSBracket, bracket_id);
      log_error_lang(stream_.token()->get_span(), token_id::RSBracket,
                     bracket_id);
      return {nullptr};
    }
  }

  return {nullptr};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_class_name() {
  auto class_name = std::make_shared<class_name_node>();

  class_name->set_identifier(parse_identifier().value);
  class_name->set_generic(parse_generic().value);

  return {class_name};
}

inline result<std::shared_ptr<class_name_node>> ast_parser::parse_extends() {
  auto tok_id = stream_.next_token_id();
  if (tok_id == token_id::Extends) {
    ++stream_;
    return parse_class_name();
  } else if (tok_id == token_id::Is) {
    return {nullptr, true};
  } else {
    logger::error("expected:", tok_to_str(token_id::Extends), ",",
                  tok_to_str(token_id::Is), ", but was", tok_to_str(tok_id));

    return {nullptr};
  }
}

inline result<std::shared_ptr<class_node>> ast_parser::parse_class() {
  auto node = std::make_shared<class_node>();

  node->set_class_name(parse_class_name().value);
  node->set_extends(parse_extends().value);

  auto& token = stream_.next_and_token();
  if (auto tok_id = token->get_token_id(); tok_id != token_id::Is) {
    log_expected_actual(token_id::Is, tok_id);
    log_error_lang(stream_.token()->get_span(), token_id::Is, tok_id);
    return {nullptr};
  }

  for (++stream_; stream_ && stream_.get_token_id() != token_id::End;
       ++stream_) {
    node->add_member(parse_member().value);
  }
  if (!stream_) {
    log_error_lang(token->get_span(), R"("end" not found for "is")");
  }
  return {node};
}

inline result<std::shared_ptr<program_node>> ast_parser::parse_program() {
  auto program = std::make_shared<program_node>();

  for (; stream_; ++stream_) {
    auto tok_id = stream_.get_token_id();
    switch (tok_id) {
      case token_id::Class: {
        program->add_class(parse_class().value);
        break;
      }

      default: {
        log_expected_actual(token_id::Class, tok_id);
        log_error_lang(stream_.token()->get_span(), token_id::Class, tok_id);
      }
    }
  }

  return {program};
}
}  // namespace

inline ast make_ast(token_vector& tokens) {
  token_vector valid_tokens;
  for (auto& token : tokens) {
    auto id = token->get_token_id();
    if ((id != token_id::Space && id != token_id::NewLine) ||
        (id == token_id::NewLine &&
         valid_tokens.back()->get_token_id() == token_id::Return))
      valid_tokens.emplace_back(std::move(token));
  }

  ast_parser parser(valid_tokens);

  ast tree(parser.parse_program().value);
  tree.print();
  return tree;
}
}  // namespace tree

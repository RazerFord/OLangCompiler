#pragma once
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "token-code.h"
#include "token.h"

class token_generator {
  const static inline std::unordered_map<std::string, token_id> token_by_name =
      {{"class", token_id::Class},
       {"extends", token_id::Extends},
       {"is", token_id::Is},
       {"end", token_id::End},
       {"method", token_id::Method},
       {"var", token_id::Var},
       {"while", token_id::While},
       {"loop", token_id::Loop},
       {"then", token_id::Then},
       {"else", token_id::Else},
       {"return", token_id::Return},
       {"this", token_id::This},
       {"null", token_id::Null},
       {"base", token_id::Base},
       {".", token_id::Dot},
       {":", token_id::Colon},
       {",", token_id::Comma},
       {"(", token_id::LBracket},
       {")", token_id::RBracket},
       {"[", token_id::LSBracket},
       {"]", token_id::RSBracket},
       {":=", token_id::Assign},
       {"true", token_id::BooleanLiteral},
       {"false", token_id::BooleanLiteral},
       {" ", token_id::Space},
       {"\n", token_id::NewLine}};


  enum class State {
    Start,
    Word,
    Numeric,
    RealNumeric,
  };

  struct generator_context {
    std::vector<std::unique_ptr<token::token>> tokens;
    std::string buff;
    int position = 0;
    State state = State::Start;
  };

  static void complete_token(generator_context& context, std::ifstream& source_code) {
     if (context.buff.empty()) return;

      context.position--;
      token::span span(context.position - context.buff.size(), context.position);
      std::unique_ptr<token::token> token;

      switch (context.state) {
        case State::Word: {
          if (token_by_name.contains(context.buff)) {
            auto token_id = token_by_name.at(context.buff);
            if (token_id == token_id::BooleanLiteral) {
              token = token::make<token::boolean_literal, bool>(span, token_id,
                                                                context.buff);
            } else {
              token = std::make_unique<token::keyword>(
                  span, token_by_name.at(context.buff), context.buff);
            }
          } else {
            token = std::make_unique<token::identifier>(
                span, token_id::Identifier, context.buff);
          }
        } break;
        case State::Numeric: {
          token = token::make<token::integer_literal, std::int32_t>(
              span, token_id::IntegerLiteral, context.buff);
        } break;
        case State::RealNumeric: {
          token = token::make<token::real_literal, std::double_t>(
              span, token_id::RealLiteral, context.buff);
          break;
        }
        default: {
        }
      }

      context.tokens.push_back(std::move(token));
      context.buff.clear();

      source_code.unget();
      context.state = State::Start;
  }

 public:
  static std::vector<std::unique_ptr<token::token>> generate_token(
      const std::string& filepath) {
    std::ifstream source_code(filepath);

    generator_context context;

    char ch;
    while (source_code.get(ch)) {
      context.position++;
      switch (context.state) {
        case State::Start: {
          context.buff += ch;
          if (std::isalpha(ch)) {
            context.state = State::Word;
          } else if (std::isdigit(ch)) {
            context.state = State::Numeric;
          } else if (ch == ':' && source_code.peek() == '=') {
            continue;
          } else {
            auto token_id = token_by_name.contains(context.buff)
                                ? token_by_name.at(context.buff)
                                : token_id::Unknown;

            token::span span(context.position - context.buff.size(), context.position);

            context.tokens.emplace_back(
                std::make_unique<token::symbol>(span, token_id, context.buff));

            context.buff.clear();
          }
          break;
        };
        case State::Word: {
          if (std::isalpha(ch) || std::isdigit(ch)) {
            context.buff += ch;
          } else {
            complete_token(context, source_code);
          }
          break;
        }
        case State::Numeric: {
          if (ch == '.') {
            context.state = State::RealNumeric;
            context.buff += ch;
            continue;
          } else if (std::isdigit(ch)) {
            context.buff += ch;
          } else {
            complete_token(context, source_code);
          }
          break;
        }
        case State::RealNumeric: {
          if (std::isdigit(ch)) {
            context.buff += ch;
          } else {
            complete_token(context, source_code);
          }
          break;
        }
        default: {
        }
      }
    }

    complete_token(context, source_code);

    return std::move(context.tokens);
  }
};
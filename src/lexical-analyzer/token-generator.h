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
 public:
  const static inline std::unordered_map<std::string, token_id> token_by_name =
      {{"class", token_id::Class},
       {"extends", token_id::Extends},
       {"is", token_id::Is},
       {"end", token_id::End},
       {"var", token_id::Var},
       {"while", token_id::While},
       {"loop", token_id::Loop},
       {"then", token_id::Then},
       {"else", token_id::Else},
       {"return", token_id::Return},
       {"this", token_id::This},
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

  static decltype(auto) generate_token(const std::string& filepath) {
    std::ifstream source_code(filepath);

    std::vector<std::unique_ptr<token::token>> tokens;

    char ch;
    std::string buff;
    int position = -1;
    State state = State::Start;

    while (source_code.get(ch)) {
      position++;
      switch (state) {
        case State::Start: {
          buff += ch;
          if (std::isalpha(ch)) {
            state = State::Word;
          } else if (std::isdigit(ch)) {
            state = State::Numeric;
          } else if (ch == ':' && source_code.peek() == '=') {
            continue;
          } else {
            auto token_id = token_by_name.contains(buff)
                                ? token_by_name.at(buff)
                                : token_id::Unknown;

            token::span span(position - buff.size(), position);

            tokens.emplace_back(
                std::make_unique<token::symbol>(span, token_id, buff));

            buff.clear();
          }
          break;
        };
        case State::Word: {
          if (!(std::isalpha(ch) && std::isdigit(ch))) {
            token::span span(position - buff.size(), position);
            std::unique_ptr<token::token> token;
            if (token_by_name.contains(buff)) {
              auto token_id = token_by_name.at(buff);
              if (token_id == token_id::BooleanLiteral) {
                token = token::make<token::boolean_literal, bool>(
                    span, token_id, buff);
              } else {
                token = std::make_unique<token::keyword>(
                    span, token_by_name.at(buff), buff);
              }
            } else {
              token = std::make_unique<token::identifier>(
                  span, token_id::Identifier, buff);
            }
            tokens.push_back(std::move(token));
            buff.clear();

            source_code.unget();
            position--;
            state = State::Start;
          } else {
            buff += ch;
          }
        }
        case State::Numeric: {
          if (ch == '.') {
            state = State::RealNumeric;
            buff += ch;
            continue;
          } else if (std::isdigit(ch)) {
            buff += ch;
          } else {
            token::span span(position - buff.size(), position);
            tokens.emplace_back(
                token::make<token::integer_literal, std::int32_t>(
                    span, token_id::IntegerLiteral, buff));
            buff.clear();

            source_code.unget();
            position--;
            state = State::Start;
          }
        }
        case State::RealNumeric: {
          if (std::isdigit(ch)) {
            buff += ch;
          } else {
            token::span span(position - buff.size(), position);

            tokens.emplace_back(token::make<token::real_literal, std::double_t>(
                span, token_id::IntegerLiteral, buff));

            buff.clear();

            source_code.unget();
            position--;
            state = State::Start;
          }
        }
        default: {
        }
      }
    }

    return tokens;
  }
};
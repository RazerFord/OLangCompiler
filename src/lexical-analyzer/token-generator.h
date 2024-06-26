#pragma once
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <memory>
#include <sstream>
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
       {"if", token_id::If},
       {"then", token_id::Then},
       {"else", token_id::Else},
       {"return", token_id::Return},
       {"this", token_id::This},
       {"null", token_id::Null},
       {"base", token_id::Base},
       {"std", token_id::Std},
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
    StringStart,
    StringEnd,
  };

  struct generator_context {
    std::vector<std::unique_ptr<token::token>> tokens;
    std::string buff;
    State state = State::Start;

    int position = 0;
    int line = 1;
    int offset = 1;

    int next() {
      offset++;
      return ++position;
    }

    int prev() {
      offset--;
      return --position;
    }

    int new_line() {
      offset = 1;
      return ++line;
    }
  };

  inline static token::span create_span(const generator_context& context) {
    return {context.position - context.buff.size(),
            static_cast<size_t>(context.position),
            static_cast<size_t>(context.line),
            context.offset - context.buff.size()};
  }

  static void complete_token(generator_context& context,
                             std::stringstream& source_code) {
    if (context.buff.empty()) return;

    context.prev();
    token::span span = create_span(context);
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
      case State::StringEnd: {
        std::string tmp_literal =
            std::string(context.buff.begin() + 1,
                        context.buff.begin() + context.buff.size() - 1);
        replace_all(tmp_literal, "\\n", "\n");
        replace_all(tmp_literal, "\\t", "\t");
        replace_all(tmp_literal, "\\b", "\b");
        token = token::make<token::string_literal, std::string>(
            span, token_id::StringLiteral, tmp_literal);
        break;
      }
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

  static void replace_first(std::string& s, std::string const& toReplace,
                            std::string const& replaceWith) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos) return;
    s.replace(pos, toReplace.length(), replaceWith);
  }

  static std::string& replace_all(std::string& s, std::string const& from,
                                  std::string const& to) {
    if (!from.empty())
      for (std::string::size_type pos = 0; (pos = s.find(from, pos) + 1);
           pos += to.size())
        s.replace(--pos, from.size(), to);
    return s;
  }

 public:
  static std::vector<std::unique_ptr<token::token>> generate_token(
      const std::string& filepath, const std::string& stdlib_path) {
    std::ifstream stdlib_code(stdlib_path);
    if (!stdlib_code.is_open()) {
      std::cout << "cannot load stdlib" << std::endl;
      return {};
    }
    std::ifstream user_code(filepath);
    std::stringstream source_code;
    source_code << user_code.rdbuf() << "\n";
    source_code << stdlib_code.rdbuf() << "\n";

    generator_context context;

    char ch;
    while (source_code.get(ch)) {
      context.next();
      switch (context.state) {
        case State::Start: {
          context.buff += ch;
          if (ch == '"') {
            context.state = State::StringStart;
          } else if (std::isalpha(ch)) {
            context.state = State::Word;
          } else if (std::isdigit(ch) || ch == '-') {
            context.state = State::Numeric;
          } else if (ch == ':' && source_code.peek() == '=') {
            continue;
          } else {
            auto token_id = token_by_name.contains(context.buff)
                                ? token_by_name.at(context.buff)
                                : token_id::Unknown;

            if (token_id == token_id::NewLine) {
              context.new_line();
            }

            token::span span = create_span(context);

            context.tokens.emplace_back(
                std::make_unique<token::symbol>(span, token_id, context.buff));

            context.buff.clear();
          }
          break;
        };
        case State::StringStart: {
          context.buff += ch;
          if (ch == '"') context.state = State::StringEnd;
          break;
        }
        case State::StringEnd: {
          complete_token(context, source_code);
          break;
        }
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

    bool valid = true;
    for (const auto& tok : context.tokens) {
      if (tok->get_token_id() == token_id::Unknown) {
        valid = false;
        const auto& span = tok->get_span();
        std::cout << span.line_ << ":" << span.offset_ << " token "
                  << token_id_to_string(tok->get_token_id()) << std::endl;
      }
    }
    if (!valid) {
      return {};
    }

    return std::move(context.tokens);
  }
};
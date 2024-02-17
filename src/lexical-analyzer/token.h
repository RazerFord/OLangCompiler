#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "token-code.h"

namespace token {
struct span {
  std::size_t begin_;
  std::size_t end_;

  span(std::size_t begin, std::size_t end) : begin_{begin}, end_{end} {}
  span(const span& other) : span(other.begin_, other.end_) {}
};

class token {
 protected:
  span span_;
  token_id code_;  // todo: replace on enum value

  template <class E>
  static decltype(auto) printable_enum(const E& e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

 public:
  token(const span& span, token_id code) : span_(span), code_(code) {}
  token(const token& other) : token(other.span_, other.code_) {}

  virtual void print() {
    std::cout << "[" << span_.begin_ << ", " << span_.end_ << "]"
              << " code: " << printable_enum(code_) << std::endl;
  }

  virtual ~token(){};
};

class identifier : public token {
 private:
  std::string value_;

 public:
  identifier(const span& span, token_id code, const std::string& value)
      : token(span, code), value_(value) {}

  identifier(const token& other, const std::string& value)
      : token(other), value_(value) {}

  identifier(const identifier& identifier)
      : token(identifier), value_(identifier.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~identifier() override {}
};

class char_literal : public token {
 private:
  char value_;

 public:
  char_literal(const span& span, token_id code, const char value)
      : token(span, code), value_(value) {}

  char_literal(const token& other, const char value)
      : token(other), value_(value) {}

  char_literal(const char_literal& char_literal)
      : token(char_literal), value_(char_literal.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~char_literal() override {}
};

class integer_literal : public token {
 private:
  std::int32_t value_;

 public:
  integer_literal(const span& span, token_id code, std::int32_t value)
      : token(span, code), value_(value) {}

  integer_literal(const token& other, std::int32_t value)
      : token(other), value_(value) {}

  integer_literal(const integer_literal& integer_literal)
      : token(integer_literal), value_(integer_literal.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~integer_literal() override {}
};

class real_literal : public token {
 private:
  std::double_t value_;

 public:
  real_literal(const span& span, token_id code, std::double_t value)
      : token(span, code), value_(value) {}

  real_literal(const token& other, std::double_t value)
      : token(other), value_(value) {}

  real_literal(const real_literal& real_literal)
      : token(real_literal), value_(real_literal.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~real_literal() override {}
};

class boolean_literal : public token {
 private:
  bool value_;

 public:
  boolean_literal(const span& span, token_id code, bool value)
      : token(span, code), value_(value) {}

  boolean_literal(const token& other, bool value)
      : token(other), value_(value) {}

  boolean_literal(const boolean_literal& boolean_literal)
      : token(boolean_literal), value_(boolean_literal.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~boolean_literal() override {}
};

class keyword : public token {
 private:
  std::string value_;

 public:
  keyword(const span& span, token_id code, const std::string& value)
      : token(span, code), value_(value) {}

  keyword(const token& other, const std::string& value)
      : token(other), value_(value) {}

  keyword(const keyword& keyword) : token(keyword), value_(keyword.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~keyword() override {}
};

class symbol : public token {
 private:
  std::string value_;

 public:
  symbol(const span& span, token_id code, const std::string& value)
      : token(span, code), value_(value) {}

  symbol(const token& other, const std::string& value)
      : token(other), value_(value) {}

  symbol(const symbol& symbol) : token(symbol), value_(symbol.value_) {}

  virtual void print() override {
    std::cout << value_ << " ";
    this->token::print();
  }

  ~symbol() override {}
};

template <class T>
class literal : token {
 private:
  T value_;

 public:
  literal(const span& span, token_id code, const T& value)
      : token(span, code), value_(value) {}

  literal(const token& other, const std::string& value)
      : token(other), value_(value) {}

  literal(const literal& literal) : token(literal), value_(literal.value_) {}

  ~literal() override {}
};

template <class T, class U>
std::unique_ptr<T> make(const span& span, token_id code,
                        const std::string& value) {
  if constexpr (std::is_same_v<U, bool>) {
    if (value == "true") {
      return std::make_unique<T>(span, code, true);
    } else if (value == "false") {
      return std::make_unique<T>(span, code, false);
    } else {
      assert(false);
    }
  } else if constexpr (std::is_same_v<U, std::int32_t>) {
    return std::make_unique<T>(span, code, std::stoi(value));
  } else if constexpr (std::is_same_v<U, std::double_t>) {
    return std::make_unique<T>(span, code, std::stod(value));
  } else if constexpr (std::is_same_v<U, std::string>) {
    return std::make_unique<T>(span, code, value);
  } else {
    static_assert(false);
  }
}
}  // namespace token
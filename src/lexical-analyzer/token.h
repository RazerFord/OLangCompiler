#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

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
  std::uint32_t code_;  // todo: replace on enum value

 public:
  token(const span& span, std::uint32_t code) : span_(span), code_(code) {}
  token(const token& other) : token(other.span_, other.code_) {}

  virtual ~token(){};
};

class identifier : public token {
 private:
  std::string value_;

 public:
  identifier(const span& span, std::uint32_t code, const std::string& value)
      : token(span, code), value_(value) {}

  identifier(const token& other, const std::string& value)
      : token(other), value_(value) {}

  identifier(const identifier& identifier)
      : token(identifier), value_(identifier.value_) {}

  ~identifier() override {}
};

class char_literal : public token {
 private:
  char value_;

 public:
  char_literal(const span& span, std::uint32_t code, const char value)
      : token(span, code), value_(value) {}

  char_literal(const token& other, const char value)
      : token(other), value_(value) {}

  char_literal(const char_literal& char_literal)
      : token(char_literal), value_(char_literal.value_) {}

  ~char_literal() override {}
};

class integer_literal : public token {
 private:
  std::int32_t value_;

 public:
  integer_literal(const span& span, std::uint32_t code, std::int32_t value)
      : token(span, code), value_(value) {}

  integer_literal(const token& other, std::int32_t value)
      : token(other), value_(value) {}

  integer_literal(const integer_literal& integer_literal)
      : token(integer_literal), value_(integer_literal.value_) {}

  ~integer_literal() override {}
};

class real_literal : public token {
 private:
  std::double_t value_;

 public:
  real_literal(const span& span, std::uint32_t code, std::double_t value)
      : token(span, code), value_(value) {}

  real_literal(const token& other, std::double_t value)
      : token(other), value_(value) {}

  real_literal(const real_literal& real_literal)
      : token(real_literal), value_(real_literal.value_) {}

  ~real_literal() override {}
};

class boolean_literal : public token {
 private:
  bool value_;

 public:
  boolean_literal(const span& span, std::uint32_t code, bool value)
      : token(span, code), value_(value) {}

  boolean_literal(const token& other, bool value)
      : token(other), value_(value) {}

  boolean_literal(const boolean_literal& boolean_literal)
      : token(boolean_literal), value_(boolean_literal.value_) {}

  ~boolean_literal() override {}
};

class keyword : public token {
 private:
  std::string value_;

 public:
  keyword(const span& span, std::uint32_t code, const std::string& value)
      : token(span, code), value_(value) {}

  keyword(const token& other, const std::string& value)
      : token(other), value_(value) {}

  keyword(const keyword& keyword) : token(keyword), value_(keyword.value_) {}

  ~keyword() override {}
};

class symbol : public token {
 private:
  std::string value_;

 public:
  symbol(const span& span, std::uint32_t code, const std::string& value)
      : token(span, code), value_(value) {}

  symbol(const token& other, const std::string& value)
      : token(other), value_(value) {}

  symbol(const symbol& symbol) : token(symbol), value_(symbol.value_) {}

  ~symbol() override {}
};
}  // namespace token
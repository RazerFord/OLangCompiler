#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ios>
#include <iostream>
#include <memory>
#include <string>

#include "token-code.h"

namespace token {
struct span {
  std::size_t begin_;
  std::size_t end_;
  std::size_t line_;
  std::size_t offset_;

  span() = default;
  span(const std::size_t begin, const std::size_t end, const std::size_t line,
       const std::size_t offset)
      : begin_{begin}, end_{end}, line_{line}, offset_{offset} {}
  span(const span& other)
      : span(other.begin_, other.end_, other.line_, other.offset_) {}

  span& merge(const span& other) {
    begin_ = std::min(begin_, other.begin_);
    end_ = std::max(end_, other.end_);
    line_ = std::min(line_, other.line_);
    offset_ = std::min(offset_, other.offset_);
    return *this;
  }

  bool operator==(const span& other) const noexcept {
    if (this == &other) {
      return true;
    }
    return begin_ == other.begin_ && end_ == other.end_ &&
           line_ == other.line_ && offset_ == other.offset_;
  }

  static span merge(const span& lspan, const span& rspan) {
    return {std::min(lspan.begin_, rspan.begin_),
            std::max(lspan.end_, rspan.end_),
            std::min(lspan.line_, rspan.line_),
            std::min(lspan.offset_, rspan.offset_)};
  }
};

class token {
 protected:
  span span_;
  token_id code_;

  template <class E>
  static decltype(auto) printable_enum(const E& e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

 public:
  token(const span& span, token_id code) : span_(span), code_(code) {}
  token(const token& other) : token(other.span_, other.code_) {}

  [[nodiscard]] token_id get_token_id() const { return code_; }

  [[nodiscard]] const span& get_span() const { return span_; }

  virtual void print() = 0;

  virtual ~token() = default;
};

template <class T>
class basic_template_token : public token {
 protected:
  T value_;

 public:
  basic_template_token(const span& span, token_id code, const T& value)
      : token(span, code), value_(value) {}

  basic_template_token(const token& other, const std::string& value)
      : token(other), value_(value) {}

  basic_template_token(const basic_template_token& btt)
      : token(btt), value_(btt.value_) {}

  [[nodiscard]] const T& get_value() const { return value_; }

  void print() override {
    std::cout << token_id_to_string(code_) << " "
              << "[" << this->span_.begin_ << ", " << this->span_.end_ << ")"
              << " \"" << this->value_ << "\"" << std::endl;
  }

  ~basic_template_token() override = default;
};

class identifier : public basic_template_token<std::string> {
 public:
  using basic_template_token::basic_template_token;
};

class keyword : public basic_template_token<std::string> {
 public:
  using basic_template_token::basic_template_token;
};

class symbol : public basic_template_token<std::string> {
 public:
  using basic_template_token::basic_template_token;

  void print() override {
    std::string tok;
    if (this->value_ == "\n") {
      tok = "\\n";
    } else {
      tok = this->value_;
    }
    std::cout << token_id_to_string(code_) << " "
              << "[" << this->span_.begin_ << ", " << this->span_.end_ << ", "
              << span_.line_ << ":" << span_.offset_ << ")"
              << " \"" << tok << "\"" << std::endl;
  }
};

class char_literal : public basic_template_token<char> {
 public:
  using basic_template_token::basic_template_token;
};

class integer_literal : public basic_template_token<std::int32_t> {
 public:
  using basic_template_token::basic_template_token;
};

class real_literal : public basic_template_token<std::double_t> {
 public:
  using basic_template_token::basic_template_token;
};

class boolean_literal : public basic_template_token<bool> {
 public:
  using basic_template_token::basic_template_token;

  void print() override {
    std::cout << std::boolalpha;
    this->basic_template_token::print();
    std::cout << std::noboolalpha << std::endl;
  }
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
  } else {
    return std::make_unique<T>(span, code, value);
  }
}
}  // namespace token
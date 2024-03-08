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

  span(std::size_t begin, std::size_t end) : begin_{begin}, end_{end} {}
  span(const span& other) : span(other.begin_, other.end_) {}
  
  span merge(const span& lspan, const span& rspan) {
    return span(std::min(lspan.begin_, rspan.begin_), std::max(lspan.end_, rspan.end_));
  }
  span& merge_in_place(const span& other) {
    begin_ = std::min(begin_, other.begin_);
    end_ = std::max(end_, other.end_);
    return *this;
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

  virtual void print() = 0;

  virtual ~token(){};
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

  void print() override {
    std::cout << token_id_to_string(code_) << " "
              << "[" << this->span_.begin_ << ", " << this->span_.end_ << ")"
              << " \"" << this->value_ << "\"" << std::endl;
  }

  ~basic_template_token() override {}
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
              << "[" << this->span_.begin_ << ", " << this->span_.end_ << ")"
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
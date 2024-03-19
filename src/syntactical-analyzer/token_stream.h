#pragma once
#include <memory>
#include <vector>

#include "./../lexical-analyzer/token.h"
#include "lexical-analyzer/token-code.h"

class token_stream {
  using token_vector = std::vector<std::unique_ptr<token::token>>;
  const token_vector& tokens_;
  size_t cursor_ = 0;

 public:
  explicit token_stream(const token_vector& tokens) : tokens_(tokens) {}

  token_stream& operator++() {
    ++cursor_;
    return *this;
  }

  token_stream& operator++(int) {
    cursor_++;
    return *this;
  }

  token_stream& operator--() {
    --cursor_;
    return *this;
  }

  token_stream& operator--(int) {
    cursor_--;
    return *this;
  }

  [[nodiscard]] const std::unique_ptr<token::token>& next_token() const {
    return tokens_.at(cursor_ + 1);
  }

  [[nodiscard]] const std::unique_ptr<token::token>& token() const {
    return tokens_.at(cursor_);
  }

  [[nodiscard]] token_id next_token_id() const {
    return tokens_.at(cursor_ + 1)->get_token_id();
  }

  const std::unique_ptr<token::token>& next_and_token() { return tokens_.at(++cursor_); }
  
  token_id next_and_token_id() { return tokens_.at(++cursor_)->get_token_id(); }


  [[nodiscard]] token_id get_token_id() const { return tokens_.at(cursor_)->get_token_id(); }

  const std::unique_ptr<token::token>& operator*() const {
    return tokens_.at(cursor_);
  }

  explicit operator bool() const { return cursor_ < tokens_.size(); }
  [[nodiscard]] size_t size() const { return tokens_.size(); }
};
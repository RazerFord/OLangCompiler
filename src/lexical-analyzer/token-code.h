#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class token_id {
  Unknown,
  Identifier,
  CharLiteral,
  Digit,
  Class,
  Extends,
  Is,
  End,
  Var,
  Colon,
  Comma,
  LBracket,
  RBracket,
  LSBracket,
  RSBracket,
  Assign,
  While,
  Loop,
  If,
  Then,
  Else,
  Return,
  This,
  Dot,
  IntegerLiteral,
  RealLiteral,
  BooleanLiteral,
  Space,
  NewLine,
};

namespace {
static std::uint8_t max_width = 15;

const std::unordered_map<token_id, std::string> string_by_token_id = {
    {token_id::Unknown, "Unknow"},
    {token_id::Identifier, "Identifier"},
    {token_id::CharLiteral, "CharLiteral"},
    {token_id::Digit, "Digit"},
    {token_id::Class, "Class"},
    {token_id::Extends, "Extends"},
    {token_id::Is, "Is"},
    {token_id::End, "End"},
    {token_id::Var, "Var"},
    {token_id::Colon, "Colon"},
    {token_id::Comma, "Comma"},
    {token_id::LBracket, "LBracket"},
    {token_id::RBracket, "RBracket"},
    {token_id::LSBracket, "LSBracket"},
    {token_id::RSBracket, "RSBracket"},
    {token_id::Assign, "Assign"},
    {token_id::While, "While"},
    {token_id::Loop, "Loop"},
    {token_id::If, "If"},
    {token_id::Then, "Then"},
    {token_id::Else, "Else"},
    {token_id::Return, "Return"},
    {token_id::This, "This"},
    {token_id::Dot, "Dot"},
    {token_id::IntegerLiteral, "IntegerLiteral"},
    {token_id::RealLiteral, "RealLiteral"},
    {token_id::BooleanLiteral, "BooleanLiteral"},
    {token_id::Space, "Space"},
    {token_id::NewLine, "NewLine"},
};
}  // namespace

inline std::string token_id_to_string(token_id token_id) {
  std::string tok = string_by_token_id.at(token_id);
  tok.resize(max_width, ' ');
  return tok;
}
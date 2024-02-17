#pragma once
#include <fstream>
#include <string>
#include <unordered_map>

#include "token-code.h"

class token_generator {
 public:
  const static inline std::unordered_map<std::string, token_id> toke_by_name = {
      {"class", token_id::Class},
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
      {" ", token_id::Space}};

  static void generate_token(const std::string& filepath) {}
};
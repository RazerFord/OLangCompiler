#pragma once

#include <memory>
#include <unordered_map>

#include "./../syntactical-analyzer/details.h"

namespace scope_checking {
namespace {
class scope_symbol {
 private:
  std::unordered_map<details::meta, std::shared_ptr<details::ast_node>>
      symbols_;

 public:
  std::shared_ptr<details::ast_node> operator[](
      const details::meta& key) const {
    return symbols_.at(key);
  }
};
}  // namespace

class scope : public std::enable_shared_from_this<scope> {
 private:
  std::shared_ptr<scope> parent_;
  std::shared_ptr<scope_symbol> symbol_;

 public:
  scope() = default;

  scope(std::shared_ptr<scope> parent) : parent_{std::move(parent)} {}

  scope(std::shared_ptr<scope> parent, std::shared_ptr<scope_symbol> symbol)
      : parent_{std::move(parent)}, symbol_{std::move(symbol)} {}

  scope(std::shared_ptr<scope_symbol> symbol)
      : parent_{}, symbol_{std::move(symbol)} {}

  std::shared_ptr<scope> push();

  std::shared_ptr<scope> push(std::shared_ptr<scope_symbol> symbols);

  std::shared_ptr<scope> pop();

  std::shared_ptr<details::ast_node> find(details::meta);
};
}  // namespace scope_checking
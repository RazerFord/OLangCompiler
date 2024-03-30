#pragma once

#include <memory>
#include <unordered_map>

#include "./../syntactical-analyzer/details.h"

namespace scope_checking {
namespace {
struct scope_symbol {
  std::unordered_map<details::meta, std::shared_ptr<details::ast_node>> symbols;
};
}  // namespace

class scope {
 private:
  std::shared_ptr<scope> parent;
  std::shared_ptr<scope_symbol> symbols;

 public:
  std::shared_ptr<scope> push();
  std::shared_ptr<scope> push(std::shared_ptr<scope_symbol> symbols);
  std::shared_ptr<scope> pop();
  std::shared_ptr<details::ast_node> find(details::meta);
};

}  // namespace scope_checking
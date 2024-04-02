#pragma once

#include <memory>
#include <unordered_map>

#include "./../syntactical-analyzer/details.h"

namespace scope {
class scope_symbol {
 private:
  struct proxy {
   private:
    std::string key_;
    scope_symbol* scope_symbol_;

   public:
    proxy(std::string key, scope_symbol* scope_symbol_)
        : key_{key}, scope_symbol_{scope_symbol_} {}

    std::weak_ptr<details::ast_node> operator=(
        std::weak_ptr<details::ast_node> value) {
      return scope_symbol_->symbols_[key_] = value;
    }

    std::weak_ptr<details::ast_node> operator*() {
      return scope_symbol_->symbols_[key_];
    }

    std::weak_ptr<details::ast_node>* operator->() {
      return &scope_symbol_->symbols_[key_];
    }

    operator bool() {
      const auto& syms = scope_symbol_->symbols_;
      return (syms.find(key_) != syms.end());
    }

    operator std::weak_ptr<details::ast_node>() const {
      return scope_symbol_->tryFind(key_);
    }
  };

  std::weak_ptr<details::ast_node> tryFind(const std::string& key) const {
    auto it = symbols_.find(key);
    if (it == symbols_.end()) {
      return {};
    }
    return it->second;
  }

 private:
  std::unordered_map<std::string, std::weak_ptr<details::ast_node>> symbols_;

 public:
  std::weak_ptr<details::ast_node> operator[](const std::string& key) const {
    return tryFind(key);
  }

  proxy operator[](const std::string& key) { return proxy(key, this); }
};

class scope : public std::enable_shared_from_this<scope> {
 private:
  std::shared_ptr<scope> parent_;
  std::shared_ptr<scope_symbol> symbol_{new scope_symbol};

 public:
  scope() = default;

  scope(std::shared_ptr<scope> parent) : parent_{std::move(parent)} {}

  scope(std::shared_ptr<scope> parent, std::shared_ptr<scope_symbol> symbol)
      : parent_{std::move(parent)}, symbol_{std::move(symbol)} {}

  explicit scope(std::shared_ptr<scope_symbol> symbol)
      : parent_{}, symbol_{std::move(symbol)} {}

  std::shared_ptr<scope> push();

  std::shared_ptr<scope> push(std::shared_ptr<scope_symbol> symbols);

  std::shared_ptr<scope> pop();

  std::shared_ptr<details::ast_node> find(const std::string&);

  std::weak_ptr<details::ast_node> add(const std::string&,
                                       std::weak_ptr<details::ast_node>);
};

inline std::shared_ptr<scope> scope::push() {
  return std::make_shared<scope>(shared_from_this());
}

inline std::shared_ptr<scope> scope::push(
    std::shared_ptr<scope_symbol> symbols) {
  return std::make_shared<scope>(shared_from_this(), symbols);
}

inline std::shared_ptr<scope> scope::pop() { return parent_; }

inline std::shared_ptr<details::ast_node> scope::find(const std::string& key) {
  for (std::shared_ptr<scope> scope = this->shared_from_this(); scope;
       scope = scope->parent_) {
    if (auto value = (*scope->symbol_)[key]; value && !value->expired()) {
      return value->lock();
    }
  }
  return nullptr;
}

inline std::weak_ptr<details::ast_node> scope::add(
    const std::string& key, std::weak_ptr<details::ast_node> value) {
  return (*symbol_)[key] = value;
}
}  // namespace scope
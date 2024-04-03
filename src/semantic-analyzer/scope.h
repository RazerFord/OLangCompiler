#pragma once

#include <memory>
#include <unordered_map>

#include "./../syntactical-analyzer/details.h"

namespace scope {
enum class scope_type {
  Class,
  Method,
  Constructor,
  Body,
};

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
  std::string name_;
  scope_type type_{scope_type::Class};

 public:
  scope() = default;

  scope(std::shared_ptr<scope> parent, const std::string& name = "",
        scope_type type = scope_type::Class)
      : parent_{std::move(parent)}, name_{name}, type_{type} {}

  scope(std::shared_ptr<scope_symbol> symbol, const std::string& name = "",
        scope_type type = scope_type::Class)
      : parent_{}, symbol_{std::move(symbol)}, name_{name}, type_{type} {}

  scope(std::shared_ptr<scope> parent, std::shared_ptr<scope_symbol> symbol,
        const std::string& name = "", scope_type type = scope_type::Class)
      : parent_{std::move(parent)},
        symbol_{std::move(symbol)},
        name_{name},
        type_{type} {}

  std::shared_ptr<scope> push(const std::string& name = "",
                              scope_type type = scope_type::Class);

  std::shared_ptr<scope> push(std::shared_ptr<scope_symbol> symbols,
                              const std::string& name = "",
                              scope_type type = scope_type::Class);

  std::shared_ptr<scope> pop();

  std::shared_ptr<details::ast_node> find(const std::string&);

  std::weak_ptr<details::ast_node> add(const std::string&,
                                       std::weak_ptr<details::ast_node>);

  inline const scope_type get_type() const noexcept { return type_; }

  inline const std::string* get_name(scope_type type) const noexcept;
  inline const std::string& get_name() const noexcept;
};

inline std::shared_ptr<scope> scope::push(const std::string& name,
                                          scope_type type) {
  return std::make_shared<scope>(shared_from_this(), name, type);
}

inline std::shared_ptr<scope> scope::push(std::shared_ptr<scope_symbol> symbols,
                                          const std::string& name,
                                          scope_type type) {
  return std::make_shared<scope>(shared_from_this(), symbols, name, type);
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
  return (*symbol_)[key] = std::move(value);
}

inline const std::string* scope::get_name(scope_type type) const noexcept {
  const scope* s = this;
  while ((s = s->parent_.get()) != nullptr) {
    if (s->type_ == type) {
      return &s->get_name();
    }
  }
  return nullptr;
}

inline const std::string& scope::get_name() const noexcept { return name_; }
}  // namespace scope
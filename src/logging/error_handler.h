#pragma once
#include "syntactical-analyzer/details.h"

namespace error_handling {

struct error_t {
  details::meta metadata;
  std::string error_message;
};

inline error_t make_error_t(const details::ast_node& node,
                            const std::string& msg) {
  return error_t{node.get_meta_info(), msg};
}

class error_handling {
  std::vector<error_t> error_metadata_;

 public:
  void register_error(const error_t& err) { error_metadata_.push_back(err); }
  void print_errors() const {
    for (auto& err : error_metadata_) {
      auto& span = err.metadata.span_;
      std::cout << span.line_ << ":" << span.offset_ << " " << err.error_message
                << std::endl;
    }
  }

  [[nodiscard]] bool error() const { return !error_metadata_.empty(); }
};
}  // namespace error_handling
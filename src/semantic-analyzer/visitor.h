#pragma once

#include "syntactical-analyzer/details.h"

namespace visitor {
class visitor {
 public:
  virtual void visit(const details::identifier_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::primary_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::class_name_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::parameter_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::parameters_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::body_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::class_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::program_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::expression_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::variable_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::method_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::constructor_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::assignment_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::while_loop_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::if_statement_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::return_statement_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::arguments_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::this_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::null_node&,
                     const error_handling::error_handling&) const = 0;
  virtual void visit(const details::base_node&,
                     const error_handling::error_handling&) const = 0;

  virtual ~visitor() = default;
};
}  // namespace visitor
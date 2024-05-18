#pragma once

#include "syntactical-analyzer/details.h"

namespace visitor {
class visitor {
 public:
  virtual void visit(details::identifier_node&){};
  virtual void visit(details::primary_node&){};
  virtual void visit(details::type_node&){};
  virtual void visit(details::class_name_node&){};
  virtual void visit(details::parameter_node&){};
  virtual void visit(details::parameters_node&){};
  virtual void visit(details::body_node&){};
  virtual void visit(details::class_node&){};
  virtual void visit(details::program_node&){};
  virtual void visit(details::expression_node&){};
  virtual void visit(details::variable_node&){};
  virtual void visit(details::method_node&){};
  virtual void visit(details::constructor_node&){};
  virtual void visit(details::assignment_node&){};
  virtual void visit(details::while_loop_node&){};
  virtual void visit(details::if_statement_node&){};
  virtual void visit(details::return_statement_node&){};
  virtual void visit(details::arguments_node&){};
  virtual void visit(details::this_node&){};
  virtual void visit(details::null_node&){};
  virtual void visit(details::base_node&){};
  virtual void visit(details::void_node&){};
  virtual void visit(details::std_node&){};
  virtual void visit(details::literal_node<int32_t>&){};
  virtual void visit(details::literal_node<bool>&){};
  virtual void visit(details::literal_node<double_t>&){};
  virtual void visit(details::literal_node<std::string>&){};
  virtual void visit(details::variable_call&){};
  virtual void visit(details::constructor_call&){};
  virtual void visit(details::method_call&){};
  virtual void visit(details::member_call&){};
  virtual void visit(details::printf_call&){};
  virtual void visit(details::std_call&) {};

  virtual ~visitor() = default;
};
}  // namespace visitor

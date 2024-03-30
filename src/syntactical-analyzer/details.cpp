#include "details.h"

#include "logging/error_handler.h"
#include "semantic-analyzer/visitor.h"

using namespace details;

void identifier_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void primary_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void class_name_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void parameter_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void parameters_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void body_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void class_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void program_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void expression_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void variable_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void method_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void constructor_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void assignment_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void while_loop_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void if_statement_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void return_statement_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void arguments_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

template <typename T>
void literal_node<T>::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

template class details::literal_node<int32_t>;
template class details::literal_node<bool>;
template class details::literal_node<double_t>;

void this_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void null_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

void base_node::visit(std::shared_ptr<visitor::visitor> v) {
  error_handling::error_handling handling;
  v->visit(*this, handling);
}

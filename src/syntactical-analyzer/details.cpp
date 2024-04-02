#include "details.h"

#include "logging/error_handler.h"
#include "semantic-analyzer/visitor.h"

using namespace details;

void identifier_node::visit(visitor::visitor* v) { v->visit(*this); }

void primary_node::visit(visitor::visitor* v) { v->visit(*this); }

void class_name_node::visit(visitor::visitor* v) { v->visit(*this); }

void parameter_node::visit(visitor::visitor* v) { v->visit(*this); }

void parameters_node::visit(visitor::visitor* v) { v->visit(*this); }

void body_node::visit(visitor::visitor* v) { v->visit(*this); }

void class_node::visit(visitor::visitor* v) { v->visit(*this); }

void program_node::visit(visitor::visitor* v) { v->visit(*this); }

void expression_node::visit(visitor::visitor* v) { v->visit(*this); }

void variable_node::visit(visitor::visitor* v) { v->visit(*this); }

void method_node::visit(visitor::visitor* v) { v->visit(*this); }

void constructor_node::visit(visitor::visitor* v) { v->visit(*this); }

void assignment_node::visit(visitor::visitor* v) { v->visit(*this); }

void while_loop_node::visit(visitor::visitor* v) { v->visit(*this); }

void if_statement_node::visit(visitor::visitor* v) { v->visit(*this); }

void return_statement_node::visit(visitor::visitor* v) { v->visit(*this); }

void arguments_node::visit(visitor::visitor* v) { v->visit(*this); }

template <typename T>
void literal_node<T>::visit(visitor::visitor* v) {
  v->visit(*this);
}

template class details::literal_node<int32_t>;
template class details::literal_node<bool>;
template class details::literal_node<double_t>;

void this_node::visit(visitor::visitor* v) { v->visit(*this); }

void null_node::visit(visitor::visitor* v) { v->visit(*this); }

void base_node::visit(visitor::visitor* v) { v->visit(*this); }



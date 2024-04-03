#include "details.h"

#include <memory>

#include "logging/error_handler.h"
#include "semantic-analyzer/visitor.h"

using namespace details;

void identifier_node::visit(visitor::visitor* v) { v->visit(*this); }

void primary_node::visit(visitor::visitor* v) { v->visit(*this); }

void type_node::visit(visitor::visitor* v) { v->visit(*this); }

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

void variable_call::visit(visitor::visitor* v) { v->visit(*this); }
void constructor_call::visit(visitor::visitor* v) { v->visit(*this); }
void method_call::visit(visitor::visitor* v) { v->visit(*this); }
void member_call::visit(visitor::visitor* v) { v->visit(*this); }

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

auto EMPTY_VAR =
    std::make_shared<variable_call>(nullptr, std::make_shared<type_node>());

std::shared_ptr<variable_node> class_node::find_var_member(
    const std::shared_ptr<identifier_node>& var_name) {
  for (auto& member : members_) {
    if (auto var = std::dynamic_pointer_cast<variable_node>(member); var) {
      if (var->get_identifier()->get_name() == var_name->get_name()) {
        return var;
      }
    }
  }
  return nullptr;
}

std::shared_ptr<constructor_node> class_node::find_ctr(
    const std::shared_ptr<arguments_node>& args) {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      auto& params = ctr->get_parameters()->get_parameters();
      bool is_same = true;
      if (params.size() == args->get_arguments().size()) {
        for (size_t i = 0; i < args->get_arguments().size(); ++i) {
          if (params[i]->get_type()->type() !=
              args->get_arguments()[i]->get_type()->type()) {
            is_same = false;
            break;
          }
        }
        if (is_same) return ctr;
      }
    }
  }
  return nullptr;
}

template <typename T>
std::shared_ptr<constructor_node> class_node::find_ctr(
    std::shared_ptr<literal_node<T>> literal) {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      auto& params = ctr->get_parameters()->get_parameters();
      if (params.size() == 1) {
        auto ctr_args_type = params[0]->get_type()->type();
        if (((std::is_same_v<T, int32_t> &&
              ctr_args_type == type_node::mangle_name(type_id::i))) ||
            (std::is_same_v<T, double_t> &&
             ctr_args_type == type_node::mangle_name(type_id::f)) ||
            (std::is_same_v<T, bool> &&
             ctr_args_type == type_node::mangle_name(type_id::b))) {
          return ctr;
        }
      }
    }
  }
  return nullptr;
}

std::shared_ptr<constructor_node> class_node::find_ctr() {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      if (ctr->get_parameters()->get_parameters().empty()) return ctr;
    }
  }
  return nullptr;
}

std::shared_ptr<method_node> class_node::find_method(
    const std::shared_ptr<identifier_node>& method_name,
    const std::shared_ptr<arguments_node>& args) {
  for (auto& member : members_) {
    if (auto method = std::dynamic_pointer_cast<method_node>(member); method) {
      auto& params = method->get_parameters()->get_parameters();
      bool is_same = true;
      if (method->get_identifier()->get_name() == method_name->get_name() &&
          params.size() == args->get_arguments().size()) {
        for (size_t i = 0; i < args->get_arguments().size(); ++i) {
          if (params[i]->get_type()->type() !=
              args->get_arguments()[i]->get_type()->type()) {
            is_same = false;
            break;
          }
        }
        if (is_same) return method;
      }
    }
  }
  return nullptr;
}

template <typename T>
std::shared_ptr<variable_call> literal_node<T>::literal_expression_handle() {
  std::shared_ptr<class_node> clazz;
  if constexpr (std::is_same_v<T, int32_t>) {
    clazz = type_node::find(type_node::mangle_name(type_id::Integer));
  } else if constexpr (std::is_same_v<T, double_t>) {
    clazz = type_node::find(type_node::mangle_name(type_id::Real));
  } else if constexpr (std::is_same_v<T, bool>) {
    clazz = type_node::find(type_node::mangle_name(type_id::Boolean));
  }

  if (!clazz) return EMPTY_VAR;
  auto literal = std::dynamic_pointer_cast<literal_node<T>>(shared_from_this());
  if (auto ctor = clazz->find_ctr(literal); ctor) {
    auto ctor_call = std::make_shared<constructor_call>(
        clazz, ctor, std::vector<std::shared_ptr<ast_node>>{literal});
    return std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
  }

  return EMPTY_VAR;
}

std::vector<std::shared_ptr<ast_node>> get_args_objects(
    auto arguments, error_handling::error_handling& error_handler) {
  std::vector<std::shared_ptr<ast_node>> args_objects;
  for (auto& arg : arguments) {
    args_objects.push_back(arg->get_object(error_handler));
  }
  return args_objects;
}

std::shared_ptr<expression_ext> expression_node::this_type_checking(
    std::shared_ptr<this_node> this_keyword,
    error_handling::error_handling& error_handler) {
  auto clazz =
      std::dynamic_pointer_cast<class_node>(scope_->find(visitor::kw_this));
  if (!expression_values.empty() && expression_values[0].second) {
    if (expression_values.size() > 1) {
      error_handler.register_error(error_handling::make_error_t(
          *this_keyword, "error: invalid constructor call\n"));
      return EMPTY_VAR;
    }

    auto ctor = clazz->find_ctr(expression_values[0].second);
    auto args = get_args_objects(expression_values[0].second->get_arguments(),
                                 error_handler);
    return std::make_shared<constructor_call>(clazz, ctor, args);
  }

  return std::make_shared<variable_call>(this_keyword, clazz->get_type());
}

std::shared_ptr<expression_ext> expression_node::get_object(
    error_handling::error_handling& error_handler) {
  if (auto null = dynamic_cast<null_node*>(primary_.get()); null) {
    if (!expression_values.empty()) {
      error_handler.register_error(error_handling::make_error_t(
          *null, "error: member reference base type 'null' is not class\n"));
      return EMPTY_VAR;
    }
    final_object_ = std::make_shared<variable_call>(
        nullptr, std::make_shared<type_node>(type_id::Null));
  }

  if (final_object_) return final_object_;

  bool is_ctor = false;

  if (auto this_keyword = std::dynamic_pointer_cast<this_node>(primary_);
      this_keyword) {
    final_object_ = this_type_checking(this_keyword, error_handler);
    if (final_object_ == EMPTY_VAR ||
        (std::dynamic_pointer_cast<constructor_call>(final_object_)))
      return final_object_;
  } else if (auto literal =
                 std::dynamic_pointer_cast<literal_base_node>(primary_);
             literal) {
    auto literal_object = literal->literal_expression_handle();
    if (literal_object == EMPTY_VAR) {
      error_handler.register_error(error_handling::make_error_t(
          *this_keyword, "error: cannot find constructor for literal\n"));
      return final_object_;
    }
    final_object_ = literal_object;
  } else if (auto var = scope_->find(
                 std::dynamic_pointer_cast<class_name_node>(primary_)
                     ->get_identifier()
                     ->get_name());
             var) {
    auto type = std::make_shared<type_node>();
    if (auto var_node = std::dynamic_pointer_cast<variable_node>(var);
        var_node) {
      type = var_node->get_type();
      final_object_ = std::make_shared<variable_call>(var, type);

    } else if (auto par_node = std::dynamic_pointer_cast<parameter_node>(var);
               par_node) {
      type = par_node->get_type();
      final_object_ = std::make_shared<variable_call>(var, type);
    } else if (auto clazz = std::dynamic_pointer_cast<class_node>(var); clazz) {
      if (!expression_values.empty() && expression_values[0].first) {
        error_handler.register_error(error_handling::make_error_t(
            *var, "error: invalid constructor call\n"));
        final_object_ = EMPTY_VAR;
        return final_object_;
      }

      is_ctor = true;
      std::shared_ptr<constructor_node> ctor;
      std::vector<std::shared_ptr<ast_node>> args = {};
      if (expression_values.empty()) {
        ctor = clazz->find_ctr();
      } else {
        ctor = clazz->find_ctr(expression_values[0].second);
        args = get_args_objects(expression_values[0].second->get_arguments(),
                                error_handler);
      }

      if (!ctor) {
        error_handler.register_error(error_handling::make_error_t(
            *var, "error: cannot find constructor\n"));
        final_object_ = EMPTY_VAR;
        return final_object_;
      }

      auto ctor_call = std::make_shared<constructor_call>(clazz, ctor, args);
      final_object_ =
          std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
    }

  } else {
    error_handler.register_error(error_handling::make_error_t(
        *primary_, "error: cannot find constructor\n"));
    return nullptr;
  }

  if (!final_object_) return nullptr;
  for (auto& [member, args] : expression_values) {
    if (is_ctor) {
      is_ctor = false;
      continue;
    }
    if (!final_object_->get_type()) break;

    auto clazz = type_node::find(final_object_->get_type()->type());
    if (!clazz) {
      error_handler.register_error(error_handling::make_error_t(
          *primary_, "error: cannot find class\n"));
      break;
    }

    if (member && !args) {
      auto member_var = clazz->find_var_member(member);
      if (member_var)
        final_object_ = std::make_shared<member_call>(
            final_object_, std::make_shared<variable_call>(
                               member_var, member_var->get_type()));
      else {
        final_object_ = std::make_shared<member_call>(final_object_, EMPTY_VAR);
        error_handler.register_error(error_handling::make_error_t(
            *primary_, "error: cannot find \"" + member->get_name() +
                           "\" variable in class"));
        break;
      }
    } else if (member && args) {
      auto member_method = clazz->find_method(member, args);
      if (member_method) {
        final_object_ = std::make_shared<member_call>(
            final_object_,
            std::make_shared<method_call>(
                clazz, member_method,
                get_args_objects(args->get_arguments(), error_handler)));
      } else {
        final_object_ = std::make_shared<member_call>(final_object_, EMPTY_VAR);
        error_handler.register_error(error_handling::make_error_t(
            *primary_, "error: cannot find \"" + member->get_name() +
                           "\" method in class"));
        break;
      }
    } else {
      error_handler.register_error(error_handling::make_error_t(
          *primary_, "\ninvalid syntax"));
    }
  }
  return final_object_;
}
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
    const std::shared_ptr<identifier_node>& var_name,
    error_handling::error_handling& error_handler) {
  auto cur_class = std::dynamic_pointer_cast<class_node>(shared_from_this());
  while (cur_class) {
    for (auto& member : cur_class->get_members()) {
      if (auto var = std::dynamic_pointer_cast<variable_node>(member); var) {
        if (var->get_identifier()->get_name() == var_name->get_name()) {
          var->get_expression()->get_type(error_handler);
          return var;
        }
      }
    }
    cur_class =
        cur_class->get_extends()
            ? std::dynamic_pointer_cast<class_node>(scope_->find(
                  cur_class->get_extends()->get_identifier()->get_name()))
            : nullptr;
  }
  return nullptr;
}

std::shared_ptr<constructor_node> class_node::find_ctr(
    const std::shared_ptr<arguments_node>& args,
    error_handling::error_handling& error_handler) {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      auto& params = ctr->get_parameters()->get_parameters();
      bool is_same = true;
      if (params.size() == args->get_arguments().size()) {
        for (size_t i = 0; i < args->get_arguments().size(); ++i) {
          if (params[i]->get_type()->type() !=
              args->get_arguments()[i]->get_type(error_handler)->type()) {
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

std::shared_ptr<constructor_node> class_node::find_ctr(
    error_handling::error_handling& error_handler) {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      if (ctr->get_parameters()->get_parameters().empty()) return ctr;
    }
  }
  return nullptr;
}

std::shared_ptr<method_node> class_node::find_method(
    const std::shared_ptr<identifier_node>& method_name,
    const std::shared_ptr<arguments_node>& args,
    error_handling::error_handling& error_handler) {
  auto cur_class = std::dynamic_pointer_cast<class_node>(shared_from_this());
  while (cur_class) {
    for (auto& member : cur_class->get_members()) {
      if (auto method = std::dynamic_pointer_cast<method_node>(member);
          method) {
        auto& params = method->get_parameters()->get_parameters();
        bool is_same = true;
        if (method->get_identifier()->get_name() == method_name->get_name() &&
            params.size() == args->get_arguments().size()) {
          for (size_t i = 0; i < args->get_arguments().size(); ++i) {
            if (params[i]->get_type()->type() !=
                args->get_arguments()[i]->get_type(error_handler)->type()) {
              is_same = false;
              break;
            }
          }
          if (is_same) return method;
        }
      }
    }

    cur_class =
        cur_class->get_extends()
            ? std::dynamic_pointer_cast<class_node>(scope_->find(
                  cur_class->get_extends()->get_identifier()->get_name()))
            : nullptr;
  }
  return nullptr;
}

template <typename T>
std::shared_ptr<expression_ext> literal_node<T>::literal_expression_handle() {
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
        clazz, ctor,
        std::vector<std::shared_ptr<ast_node>>{
            std::make_shared<variable_call>(literal, clazz->get_type())},
        clazz->get_type());
    //    return std::make_shared<variable_call>(ctor_call,
    //    ctor_call->get_type());
    return ctor_call;
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

std::shared_ptr<expression_ext> expression_node::base_checking(
    std::shared_ptr<base_node> base_keyword,
    error_handling::error_handling& error_handler) {
  const std::string* base_class_name;
  if (base_class_name = scope_->get_name(scope::scope_type::Class);
      !base_class_name) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword, "error: cannot find class name"));
    return EMPTY_VAR;
  }
  std::shared_ptr<class_node> clazz;
  if (clazz =
          std::dynamic_pointer_cast<class_node>(scope_->find(*base_class_name));
      !clazz) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword, "error: cannot find class"));
    return EMPTY_VAR;
  }

  if (!clazz->get_extends()) {
    error_handler.register_error(error_handling::make_error_t(
        *clazz, "error: class not extends from anyone"));
    return EMPTY_VAR;
  }

  std::shared_ptr<class_node> extends_clazz;
  if (extends_clazz = std::dynamic_pointer_cast<class_node>(
          scope_->find(clazz->get_extends()->get_identifier()->get_name()));
      !extends_clazz) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword, "error: cannot find class of extends"));
    return EMPTY_VAR;
  }

  if (expression_values.empty() || expression_values.size() > 1 ||
      expression_values[0].first) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword, "error: invalid base constructor call"));
    return EMPTY_VAR;
  }

  if (scope_->get_type() != scope::scope_type::Constructor) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword,
        "error: cannot call the base constructor from a method"));
    return EMPTY_VAR;
  }

  auto ctor =
      extends_clazz->find_ctr(expression_values[0].second, error_handler);
  auto args = get_args_objects(expression_values[0].second->get_arguments(),
                               error_handler);

  if (!ctor) {
    error_handler.register_error(error_handling::make_error_t(
        *base_keyword, "error: cannot find constructor"));
    return EMPTY_VAR;
  }

  return std::make_shared<constructor_call>(
      extends_clazz, ctor, args,
      std::make_shared<type_node>(type_id::BaseCall));
}

std::shared_ptr<expression_ext> expression_node::this_type_checking(
    const std::shared_ptr<this_node>& this_keyword,
    error_handling::error_handling& error_handler) {
  auto clazz =
      std::dynamic_pointer_cast<class_node>(scope_->find(visitor::kw_this));
  if (!expression_values.empty() && !expression_values[0].first &&
      expression_values[0].second) {
    if (expression_values.size() > 1) {
      error_handler.register_error(error_handling::make_error_t(
          *this_keyword, "error: invalid constructor call"));
      return EMPTY_VAR;
    }

    auto ctor = clazz->find_ctr(expression_values[0].second, error_handler);
    auto args = get_args_objects(expression_values[0].second->get_arguments(),
                                 error_handler);
    if (ctor) {
      error_handler.register_error(error_handling::make_error_t(
          *this_keyword, "error: cannot call another constructor"));
    } else {
      error_handler.register_error(error_handling::make_error_t(
          *this_keyword, "error: cannot call another undeclared constructor"));
    }

    return EMPTY_VAR;
  }

  return std::make_shared<variable_call>(this_keyword, clazz->get_type());
}

std::shared_ptr<expression_ext> expression_node::constr_call_checking(
    std::shared_ptr<ast_node> constr_call, std::shared_ptr<class_node> clazz,
    error_handling::error_handling& error_handler) {
  if (!expression_values.empty() && expression_values[0].first) {
    error_handler.register_error(error_handling::make_error_t(
        *constr_call, "error: invalid constructor call\n"));
    return EMPTY_VAR;
  }

  std::shared_ptr<constructor_node> ctor;
  std::vector<std::shared_ptr<ast_node>> args = {};
  if (expression_values.empty()) {
    ctor = clazz->find_ctr(error_handler);
  } else {
    ctor = clazz->find_ctr(expression_values[0].second, error_handler);
    args = get_args_objects(expression_values[0].second->get_arguments(),
                            error_handler);
  }

  if (!ctor) {
    error_handler.register_error(error_handling::make_error_t(
        *constr_call, "error: cannot find constructor\n"));
    return EMPTY_VAR;
  }

  auto ctor_call =
      std::make_shared<constructor_call>(clazz, ctor, args, clazz->get_type());
  return ctor_call;
  //  return std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
}

std::shared_ptr<expression_ext> expression_node::get_object(
    error_handling::error_handling& error_handler) {
  if (auto null = dynamic_cast<null_node*>(primary_.get()); null) {
    if (!expression_values.empty()) {
      error_handler.register_error(error_handling::make_error_t(
          *null, "error: member reference base type 'null' is not class"));
      return EMPTY_VAR;
    }
    final_object_ = std::make_shared<variable_call>(
        nullptr, std::make_shared<type_node>(type_id::Null));
  }

  if (final_object_) return final_object_;

  final_object_ = EMPTY_VAR;

  bool is_ctor = false;
  if (auto base_keyword = std::dynamic_pointer_cast<base_node>(primary_);
      base_keyword) {
    final_object_ = base_checking(base_keyword, error_handler);
    return final_object_;
  } else if (auto this_keyword = std::dynamic_pointer_cast<this_node>(primary_);
             this_keyword) {
    final_object_ = this_type_checking(this_keyword, error_handler);
    if (final_object_ == EMPTY_VAR) return final_object_;
  } else if (auto literal =
                 std::dynamic_pointer_cast<literal_base_node>(primary_);
             literal) {
    if (final_object_ = literal->literal_expression_handle();
        final_object_ == EMPTY_VAR) {
      error_handler.register_error(error_handling::make_error_t(
          *literal, "error: cannot find constructor for literal"));
      return final_object_;
    }
  } else if (auto var = scope_->find(
                 std::dynamic_pointer_cast<class_name_node>(primary_)
                     ->get_identifier()
                     ->get_name());
             var) {
    auto type = std::make_shared<type_node>();
    if (auto var_node = std::dynamic_pointer_cast<variable_node>(var);
        var_node) {
      type = var_node->get_type();
      final_object_ = std::make_shared<variable_call>(var_node, type);

    } else if (auto par_node = std::dynamic_pointer_cast<parameter_node>(var);
               par_node) {
      type = par_node->get_type();
      final_object_ = std::make_shared<variable_call>(par_node, type);
    } else if (auto clazz = std::dynamic_pointer_cast<class_node>(var); clazz) {
      is_ctor = true;
      final_object_ = constr_call_checking(var, clazz, error_handler);
    } else {
      error_handler.register_error(error_handling::make_error_t(
          *var, "error: cannot deduce the expression"));
    }

  } else {
    error_handler.register_error(error_handling::make_error_t(
        *primary_, "error: cannot find constructor\n"));
    return final_object_;
  }

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

    std::shared_ptr<expression_ext> member_call_obj;
    if (member && !args) {
      auto member_var = clazz->find_var_member(member, error_handler);
      if (!member_var) {
        final_object_ = std::make_shared<member_call>(final_object_, EMPTY_VAR);
        error_handler.register_error(error_handling::make_error_t(
            *primary_, "error: cannot find \"" + member->get_name() +
                           "\" variable in class"));
        break;
      }

      member_call_obj =
          std::make_shared<variable_call>(member_var, member_var->get_type());
    } else if (member && args) {
      auto member_method = clazz->find_method(member, args, error_handler);
      if (!member_method) {
        final_object_ = std::make_shared<member_call>(final_object_, EMPTY_VAR);
        error_handler.register_error(error_handling::make_error_t(
            *primary_, "error: cannot find \"" + member->get_name() +
                           "\" method in class"));
        break;
      }
      member_call_obj = std::make_shared<method_call>(
          clazz, member_method,
          get_args_objects(args->get_arguments(), error_handler));

    } else {
      error_handler.register_error(
          error_handling::make_error_t(*primary_, "invalid syntax"));
      break;
    }

    final_object_ =
        std::make_shared<member_call>(final_object_, member_call_obj);
  }
  return final_object_;
}
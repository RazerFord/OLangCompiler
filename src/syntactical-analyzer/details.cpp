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
void method_call::visit(visitor::visitor* v) { v-> visit(*this); }
void member_call::visit(visitor::visitor* v) { v-> visit(*this); }

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

std::shared_ptr<variable_node> class_node::find_var_member(
    std::shared_ptr<identifier_node> var_name) {
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
    std::shared_ptr<arguments_node> args) {
  for (auto& member : members_) {
    if (auto ctr = std::dynamic_pointer_cast<constructor_node>(member); ctr) {
      auto& params = ctr->get_parameters()->get_parameters();
      bool is_same = true;
      if (params.size() == args->get_arguments().size()) {
        for (size_t i = 0; i < args->get_arguments().size(); ++i) {
          if (params[i]->get_type() != args->get_arguments()[i]->get_type()) {
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
        if (((std::is_same_v<T, int32_t> && ctr_args_type == "int")) ||
            (std::is_same_v<T, double_t> && ctr_args_type == "double") ||
            (std::is_same_v<T, bool> && ctr_args_type == "bool")) {
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
      if (ctr->get_parameters()->get_parameters().empty())
        return ctr;
    }
  }
  return nullptr;
}

std::shared_ptr<method_node> class_node::find_method(
    std::shared_ptr<identifier_node> method_name,
    std::shared_ptr<arguments_node> args) {
  for (auto& member : members_) {
    if (auto method = std::dynamic_pointer_cast<method_node>(member); method) {
      auto& params = method->get_parameters()->get_parameters();
      bool is_same = false;
      if (method->get_identifier()->get_name() == method_name->get_name() &&
          params.size() == args->get_arguments().size()) {
        for (size_t i = 0; i < args->get_arguments().size(); ++i) {
          if (params[i]->get_type() != args->get_arguments()[i]->get_type()) {
            is_same = true;
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
std::shared_ptr<variable_call> literal_expression_handle(
    std::shared_ptr<details::literal_node<T>> literal) {
  std::shared_ptr<class_node> clazz;
  if constexpr (std::is_same_v<T, int32_t>) {
    clazz = type_node::find("Integer");
  } else if constexpr (std::is_same_v<T, double_t>) {
    clazz = type_node::find("Real");
  } else if constexpr (std::is_same_v<T, bool>) {
    clazz = type_node::find("Bool");
  }
  if (!literal || !clazz)
    return {};
  auto ctor = clazz->find_ctr(literal);
  if (ctor) {
    auto ctor_call = std::make_shared<constructor_call>(
        clazz, ctor, std::vector<std::shared_ptr<ast_node>>{literal});
    return std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
  } else {
    std::cout << "cannot find ctor for literal\n";
  }

  return {};
}

std::shared_ptr<expression_ext> expression_node::get_object() {
  if (auto null = dynamic_cast<null_node*>(primary_.get()); null) {
    if (!expression_values.empty()) {
      std::cout << "error: member reference base type 'null' is not class\n";
      return nullptr;
    }
  }

  if (final_object_) return final_object_;

  // todo this

  std::shared_ptr<variable_call> object;
  bool is_ctor = false;

  if (auto this_keyword = dynamic_cast<this_node*>(primary_.get());
      this_keyword) {
    object = std::make_shared<variable_call>(primary_, nullptr);
  } else if (auto literal =
                 std::dynamic_pointer_cast<literal_node<int32_t>>(primary_);
             literal) {
    object = literal_expression_handle(literal);
  } else if (auto literal =
                 std::dynamic_pointer_cast<literal_node<double_t>>(primary_);
             literal) {
    object = literal_expression_handle(literal);
  } else if (auto literal =
                 std::dynamic_pointer_cast<literal_node<bool>>(primary_);
             literal) {
    object = literal_expression_handle(literal);
  } else {
    auto class_name = dynamic_cast<class_name_node*>(primary_.get());
    if (auto var = scope_->find(class_name->get_identifier()->get_name());
        var) {
      std::shared_ptr<type_node> type;
      if (auto var_node = dynamic_cast<variable_node*>(var.get()); var_node)
        type = var_node->get_type();
      else if (auto par_node = dynamic_cast<parameter_node*>(var.get());
               par_node)
        type = par_node->get_type();

      object = std::make_shared<variable_call>(var, type);
    } else if (auto clazz = type_node::find(class_name->mangle_class_name());
               clazz) {
      if (!expression_values.empty() && expression_values[0].first) {
        std::cout << "cannot call cotr\n";
        return nullptr;
      }
      is_ctor = true;
      if (expression_values.empty()) {
        auto ctor = clazz->find_ctr();
        auto ctor_call = std::make_shared<constructor_call>(
            clazz, ctor, std::vector<std::shared_ptr<ast_node>>{});
        object =
            std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
      } else {
        auto ctor = clazz->find_ctr(expression_values[0].second);
        std::vector<std::shared_ptr<ast_node>> args_objects;
        for (auto& arg : expression_values[0].second->get_arguments()) {
          args_objects.push_back(arg->get_object());
        }
        auto ctor_call =
            std::make_shared<constructor_call>(clazz, ctor, args_objects);
        object =
            std::make_shared<variable_call>(ctor_call, ctor_call->get_type());
      }
    } else {
      std::cout << "use undeclared identifier\n";
    }
  }

  final_object_ = object;
  if (!final_object_)
    return nullptr;
  for (auto& [member, args] : expression_values) {
    if (is_ctor) continue;

    auto clazz = type_node::find(final_object_->get_type()->type());
    if (!clazz) {
      std::cout << "cannot find class\n";
      break;
    }

    if (member && !args) {
      auto member_var = clazz->find_var_member(member);
      if (member_var)
        final_object_ = std::make_shared<member_call>(
            final_object_, std::make_shared<variable_call>(
                             member_var, member_var->get_type()));
      else {
        std::cout << "cannot find \"" << member->get_name()
                  << "\" variable in class";
      }
    } else if (member && args) {
      auto member_method = clazz->find_method(member, args);
      if (member_method) {
        std::vector<std::shared_ptr<ast_node>> args_objects;
        for (auto& arg : args->get_arguments()) {
          args_objects.push_back(arg->get_object());
        }

        final_object_ = std::make_shared<member_call>(
            final_object_,
            std::make_shared<method_call>(clazz, member_method, args_objects));
      } else {
        std::cout << "cannot find \"" << member->get_name()
                  << "\" method in class";
      }
    }
  }
  return final_object_;
}
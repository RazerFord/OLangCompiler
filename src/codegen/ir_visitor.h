#pragma once

#include <set>
#include <sstream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "visitor/visitor.h"

inline constexpr std::string ModuleName = "Main";
inline constexpr std::string CtorName = "<init>";
inline constexpr std::string PrefixNameMemberVar = "_var";
inline constexpr std::string Malloc = "malloc";
inline constexpr std::string PrefixVTable = "__VTable";

namespace ir_visitor {
class ir_visitor : public visitor::visitor {
 private:
  std::unique_ptr<llvm::LLVMContext> ctx_ =
      std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::IRBuilder<>> builder_ =
      std::make_unique<llvm::IRBuilder<>>(*ctx_);
  std::unique_ptr<llvm::Module> module_ =
      std::make_unique<llvm::Module>(ModuleName, *ctx_);
  std::unordered_map<std::string, llvm::Value*> var_env;

 public:
  void visit(details::program_node& p) override {
    register_global_funcs(p);
    register_types(p);
    register_members(p);
    register_vtables(p);
    generate_def_funcs(p);

    module_->dump();
  }

 private:
  inline static std::string vtable_name(
      const std::shared_ptr<details::class_node>& c) {
    return PrefixVTable + c->get_class_name()->get_identifier()->get_name();
  }

  inline static std::vector<std::shared_ptr<details::member_node>> linelize(
      details::program_node& p, const std::shared_ptr<details::class_node>& c) {
    auto& tc = p.get_type_casting();
    std::string main_type = c->get_class_name()->get_identifier()->get_name();
    auto& ts1 = tc.at(main_type);
    std::vector<std::string> ts;
    for (const auto& [k, _] : ts1) {
      ts.push_back(k);
    }
    std::sort(ts.begin(), ts.end(),
              [&tc, &main_type](const std::string& l, const std::string& r) {
                return l == main_type ||
                       (tc.contains(l) && tc.at(l).contains(r));
              });
    std::vector<std::shared_ptr<details::member_node>> members;
    {
      std::set<std::string> added;
      const auto& scope = p.get_scope();
      for (const std::string& cls : ts) {
        auto cn = scope->find(cls);
        if (auto sp = std::dynamic_pointer_cast<details::class_node>(cn)) {
          for (const auto& m : sp->get_members()) {
            if (auto sp1 = std::dynamic_pointer_cast<details::method_node>(m)) {
              if (added.insert(sp1->mangle_method()).second) {
                members.push_back(m);
              }
            }
          }
        }
      }
    }
    return members;
  }

  void register_types(details::program_node& p) {
    for (const auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
      name = vtable_name(c);
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
    }
  }

  void register_members(details::program_node& p) {
    for (auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      llvm::StructType* ptr_cls =
          llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name));
      c->set_class_type(ptr_cls);
      name = vtable_name(c);
      llvm::PointerType* ptrVTable =
          llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name))
              ->getPointerTo();

      std::vector<llvm::Type*> body{ptrVTable};
      // adding all types to a class
      {
        member_variable_visitor mvv(&body, this);
        for (const auto& m : c->get_members()) {
          m->visit(&mvv);
        }
      }
      ptr_cls->setBody(llvm::ArrayRef(body));

      // adding all methods to a class
      {
        member_method_visitor mmv(ptr_cls, this);
        for (const auto& m : c->get_members()) {
          m->visit(&mmv);
        }
      }
    }
  }

  void register_global_funcs(details::program_node& p) {
    llvm::Type* ptr_void = llvm::Type::getInt8Ty(*ctx_)->getPointerTo();

    module_->getOrInsertFunction(
        Malloc, llvm::FunctionType::get(
                    ptr_void, llvm::IntegerType::getInt64Ty(*ctx_), false));
  }

  void register_vtables(details::program_node& p) {
    for (const auto& c : p.get_classes()) {
      std::string vtable = vtable_name(c);
      auto ptr_table = llvm::StructType::getTypeByName(*ctx_, vtable);
      std::vector<llvm::Constant*> methods;
      std::vector<llvm::Type*> types;
      vtable_filler_visitor vfv(&methods, &types, this);

      std::vector<std::shared_ptr<details::member_node>> members =
          linelize(p, c);
      for (const auto& m : members) {
        m->visit(&vfv);
      }
      ptr_table->setBody(llvm::ArrayRef(types));
      module_->getOrInsertGlobal(vtable, ptr_table);
      auto global_table = module_->getNamedGlobal(vtable);
      global_table->setInitializer(
          llvm::ConstantStruct::get(ptr_table, llvm::ArrayRef(methods)));
    }
  }

  void generate_def_funcs(details::program_node& p) {
    std::unordered_map<std::string, std::vector<details::variable_node*>>
        cls_to_vars;
    for (const auto& c : p.get_classes()) {
      std::vector<details::variable_node*> vars;
      variable_collector_visitor vcv(this, &vars);
      for (const auto& m : c->get_members()) {
        m->visit(&vcv);
      }
      cls_to_vars[c->get_class_name()->get_identifier()->get_name()] =
          std::move(vars);
    }

    func_def_visitor fdv(this, &cls_to_vars);
    for (const auto& c : p.get_classes()) {
      for (const auto& m : c->get_members()) {
        m->visit(&fdv);
      }
    }
  }

  class member_variable_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Type*>* const body_ = nullptr;
    ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_variable_visitor(std::vector<llvm::Type*>* const body,
                                     ir_visitor* ir_visitor)
        : body_{body}, ir_visitor_{ir_visitor} {}

    void visit(details::variable_node& v) override {
      std::string cls_name{
          v.get_type()->get_class_name()->get_identifier()->get_name()};
      llvm::Type* ptrCls = llvm::StructType::getTypeByName(
          *ir_visitor_->ctx_, llvm::StringRef(cls_name));
      body_->push_back(ptrCls);
    }
  };

  class member_method_visitor : public visitor::visitor {
   private:
    const llvm::StructType* const ptr_cls_ = nullptr;
    const ir_visitor* ir_visitor_ = nullptr;

   public:
    explicit member_method_visitor(const llvm::StructType* const ptr_cls,
                                   const ir_visitor* ir_visitor)
        : ptr_cls_{ptr_cls}, ir_visitor_{ir_visitor} {}

    void visit(details::method_node& f) override {
      std::vector<llvm::Type*> params;
      params.push_back(ptr_cls_->getPointerTo());
      for (const auto& p : f.get_parameters()->get_parameters()) {
        std::string cls_name = p->get_type()->simple_type();
        llvm::Type* ptr_cls = llvm::StructType::getTypeByName(
            *ir_visitor_->ctx_, llvm::StringRef(cls_name));
        params.push_back(ptr_cls);
      }
      std::string ret_type_name = f.get_return_type()->simple_type();
      llvm::Type* ret_type = llvm::StructType::getTypeByName(
          *ir_visitor_->ctx_, llvm::StringRef(ret_type_name));

      if (!ret_type) {
        ret_type = llvm::Type::getVoidTy(*ir_visitor_->ctx_);
      }

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);

      llvm::Function* func = llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          f.get_identifier()->get_name(), *ir_visitor_->module_);

      f.set_method_value(func);
    }

    void visit(details::constructor_node& c) override {
      std::vector<llvm::Type*> params;
      params.push_back(ptr_cls_->getPointerTo());
      for (const auto& p : c.get_parameters()->get_parameters()) {
        std::string cls_name = p->get_type()->simple_type();
        llvm::Type* ptr_cls = llvm::StructType::getTypeByName(
            *ir_visitor_->ctx_, llvm::StringRef(cls_name));
        params.push_back(ptr_cls);
      }
      const std::string* name =
          c.get_scope()->get_name(scope::scope_type::Class);
      auto ret_type = llvm::StructType::getTypeByName(*ir_visitor_->ctx_,
                                                      llvm::StringRef(*name));

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);

      auto func_value_ptr = llvm::Function::Create(
          ptr_func_type, llvm::Function::LinkageTypes::ExternalLinkage,
          CtorName, *ir_visitor_->module_);

      c.set_constr_type(func_value_ptr);
    }
  };

  class vtable_filler_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Constant*>* const methods_ = nullptr;
    std::vector<llvm::Type*>* const types_ = nullptr;

   public:
    explicit vtable_filler_visitor(std::vector<llvm::Constant*>* const methods,
                                   std::vector<llvm::Type*>* const types,
                                   const ir_visitor* ir_visitor)
        : methods_{methods}, types_{types} {}

    void visit(details::method_node& f) override {
      methods_->push_back(f.get_method_value());
      types_->push_back(f.get_method_value()->getReturnType());
    }
  };

  class variable_collector_visitor : public visitor::visitor {
    ir_visitor* ir_visitor_ = nullptr;
    std::vector<details::variable_node*>* vars_;

   public:
    explicit variable_collector_visitor(
        ir_visitor* ir_visitor, std::vector<details::variable_node*>* vars)
        : ir_visitor_(ir_visitor), vars_(vars) {}

    void visit(details::variable_node& node) override {
      vars_->push_back(&node);
    }
  };

  class func_def_visitor : public visitor::visitor {
    ir_visitor* const ir_visitor_ = nullptr;
    std::unordered_map<std::string, std::vector<details::variable_node*>>* const
        cls_to_vars_ = nullptr;

   public:
    explicit func_def_visitor(
        ir_visitor* ir_visitor,
        std::unordered_map<std::string, std::vector<details::variable_node*>>*
            cls_to_vars)
        : ir_visitor_(ir_visitor), cls_to_vars_(cls_to_vars) {}

    void visit(details::method_node& method) override {
      auto func_value = method.get_method_value();
      generate_def_func(func_value, method.get_parameters()->get_parameters());

      // generate expr  and set return value
      body_visitor bd_visitor(ir_visitor_, cls_to_vars_);
      method.get_body()->visit(&bd_visitor);
      llvm::verifyFunction(*func_value);
    }

    void visit(details::constructor_node& constr) override {
      auto func_value = constr.get_constr_value();
      generate_def_func(func_value, constr.get_parameters()->get_parameters());

      //      llvm::Value* return_value;
      //      if (!func_value->getReturnType()->isVoidTy()) {
      //        return_value = ir_visitor_->builder_->CreateAlloca(
      //            func_value->getReturnType(), nullptr, "return.value");
      //      }

      // generate expr  and set return value
      body_visitor bd_visitor(ir_visitor_, cls_to_vars_);
      constr.get_body()->visit(&bd_visitor);
      llvm::verifyFunction(*func_value);
    }

   private:
    void generate_def_func(
        llvm::Function* func_value,
        std::vector<std::shared_ptr<details::parameter_node>>& parameters) {
      llvm::BasicBlock* entry_basic_block =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "entry", func_value);
      ir_visitor_->builder_->SetInsertPoint(entry_basic_block);

      ir_visitor_->var_env.clear();
      for (auto& param : func_value->args()) {
        size_t paramNo = param.getArgNo();
        if (paramNo == 0) {
          ir_visitor_->var_env["this"] = &param;
          continue;
        }

        auto param_name = parameters[paramNo - 1]->get_identifier()->get_name();
        param.setName(param_name);
        llvm::Type* param_type = param.getType();
        ir_visitor_->var_env[param_name] = ir_visitor_->builder_->CreateAlloca(
            param_type, nullptr, llvm::Twine(param_name));
        parameters[paramNo - 1]->set_param_value(
            ir_visitor_->var_env[param_name]);
        ir_visitor_->builder_->CreateStore(&param,
                                           ir_visitor_->var_env[param_name]);
      }
    }
  };

  class body_visitor : public visitor::visitor {
    ir_visitor* const ir_visitor_ = nullptr;
    std::unordered_map<std::string, std::vector<details::variable_node*>>* const
        cls_to_vars_ = nullptr;

   public:
    explicit body_visitor(
        ir_visitor* const ir_visitor,
        std::unordered_map<std::string,
                           std::vector<details::variable_node*>>* const
            cls_to_vars)
        : ir_visitor_{ir_visitor}, cls_to_vars_{cls_to_vars} {}

    void visit(details::variable_node& variable) override {
      variable.get_expression()->visit(this);
      auto bound_val =
          variable.get_expression()->get_final_object()->get_value();

      auto var_name = variable.get_identifier()->get_name();
      llvm::Function* parent_function =
          ir_visitor_->builder_->GetInsertBlock()->getParent();
      llvm::IRBuilder<> tmp_builder(&(parent_function->getEntryBlock()),
                                    parent_function->getEntryBlock().begin());
      llvm::AllocaInst* var = tmp_builder.CreateAlloca(
          bound_val->getType(), nullptr, llvm::Twine(var_name));
      ir_visitor_->var_env[var_name] = var;
      ir_visitor_->builder_->CreateStore(bound_val, var);

      variable.set_value(var);
    }

    void visit(details::expression_node& expression) override {
      expression.get_final_object()->visit(this);
      expression.set_value(expression.get_final_object()->get_value());
    }

    void visit(details::body_node& body) override {
      for (auto& expr : body.get_nodes()) {
        expr->visit(this);
      }
    }

    void visit(details::return_statement_node& return_expr) override {
      auto ret_type =
          return_expr.get_expression()->get_final_object()->get_value();
      if (ret_type->getType()->isVoidTy())
        ir_visitor_->builder_->CreateRetVoid();
      else
        ir_visitor_->builder_->CreateRet(ret_type);
    }

    void visit(details::member_call& member) override {
      member.get_object()->visit(this);
      member.get_member()->visit(this);
      // make o
    }

    void visit(details::variable_call& variable) override {
      std::cout << "variable call\n";
      if (auto this_keyword = std::dynamic_pointer_cast<details::this_node>(
              variable.get_variable());
          this_keyword) {
        variable.set_value(ir_visitor_->var_env["this"]);
      } else if (auto expr = std::dynamic_pointer_cast<details::expression_ext>(
                     variable.get_variable());
                 expr) {
        expr->visit(this);
        std::cout << "expr visit in variable node\n";

      } else if (auto par_node =
                     std::dynamic_pointer_cast<details::parameter_node>(
                         variable.get_variable());
                 par_node) {
        variable.set_value(par_node->get_param_value());
      } else if (auto var_node =
                     std::dynamic_pointer_cast<details::variable_node>(
                         variable.get_variable());
                 var_node) {
        variable.set_value(var_node->get_value());
      }
    }

    void visit(details::constructor_call& constr_call) override {
      std::cout << "constr call visit\n";
      if (constr_call.get_type()->simple_type() == "base") {
        // handle base call
        return;
      }
      auto obj = create_object(constr_call);

      auto callee_fun = constr_call.get_constructor()->get_constr_value();
      if (!callee_fun) {
        std::cout << "Function doesn't exist\n";
        return;
      }

      //      auto callee_fun_type = callee_fun->getFunctionType();
      std::vector<llvm::Value*> arg_values{obj};
      auto args = constr_call.get_arguments();
      for (size_t i = 0; i < args.size(); i++) {
        args[i]->visit(this);
        llvm::Value* arg_val =
            std::dynamic_pointer_cast<details::expression_ext>(args[i])
                ->get_value();
        if (arg_val == nullptr) {
          std::cout << "Null Argument when calling function \n";
          return;
        }

        //        llvm::Type* param_type = callee_fun_type->getParamType(i);
        //        if (param_type == nullptr) {
        //          std::cout << "here\n";
        //          continue;
        //        }
        //        llvm::Value* bit_cast_arg_val =
        //            ir_visitor_->builder_->CreateBitCast(arg_val, param_type);
        arg_values.push_back(arg_val);
      }

      ir_visitor_->builder_->CreateCall(callee_fun, arg_values);

      constr_call.set_value(obj);
    }

    void visit(details::method_call& method_call) override {
      auto callee_fun = method_call.get_method()->get_method_value();
      if (!callee_fun) {
        std::cout << "Function doesn't exist\n";
        return;
      }

      auto callee_fun_type = callee_fun->getFunctionType();
      std::vector<llvm::Value*> arg_values;
      auto args = method_call.get_arguments();
      for (size_t i = 0; i < args.size(); i++) {
        args[i]->visit(this);
        llvm::Value* arg_val =
            std::dynamic_pointer_cast<details::expression_ext>(args[i])
                ->get_value();
        if (arg_val == nullptr) {
          std::cout << "Null Argument when calling function \n";
          return;
        }

        llvm::Type* paramTy = callee_fun_type->getParamType(i);
        llvm::Value* bitCastArgVal =
            ir_visitor_->builder_->CreateBitCast(arg_val, paramTy);
        arg_values.push_back(bitCastArgVal);
      }
      method_call.set_value(
          ir_visitor_->builder_->CreateCall(callee_fun, arg_values));
    }

   private:
    llvm::Value* create_object(details::constructor_call& constr_call) {
      std::string type_name = constr_call.get_type()->simple_type();
      llvm::Type* type =
          llvm::StructType::getTypeByName(*ir_visitor_->ctx_, type_name);

      llvm::Type* int64ty = llvm::Type::getInt64Ty(*ir_visitor_->ctx_);
      llvm::Value* value = llvm::Constant::getNullValue(type->getPointerTo());
      llvm::Value* ptr_obj_size = ir_visitor_->builder_->CreateConstGEP1_64(
          int64ty, value, 1, llvm::Twine("obj_size"));

      ptr_obj_size =
          ir_visitor_->builder_->CreatePointerCast(ptr_obj_size, int64ty);
      llvm::Value* ptr_obj = ir_visitor_->builder_->CreateCall(
          ir_visitor_->module_->getFunction(Malloc), ptr_obj_size);

      ptr_obj = ir_visitor_->builder_->CreatePointerCast(ptr_obj,
                                                         type->getPointerTo());

      std::string vtable_name = PrefixVTable + type_name;
      llvm::Value* vtable = ir_visitor_->module_->getNamedGlobal(vtable_name);
      llvm::Value* vtable_field =
          ir_visitor_->builder_->CreateStructGEP(type, ptr_obj, 0);

      ir_visitor_->builder_->CreateStore(vtable, vtable_field);

      // Create vars in class
      std::vector<llvm::Value*> values;
      variable_def_visitor av(&values, ir_visitor_);
      for (auto& v : cls_to_vars_->at(type_name)) {
        v->get_expression()->visit(this);
        values.push_back(v->get_expression()->get_value());
      }

      for (std::size_t i = 0; i < values.size(); i++) {
        // +1 from the table of virtual functions
        llvm::Value* field =
            ir_visitor_->builder_->CreateStructGEP(type, ptr_obj, 1 + i);
        ir_visitor_->builder_->CreateStore(values[i], field);
      }

      return ptr_obj;
    }
  };

  class variable_def_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Value*>* const values_ = nullptr;
    ir_visitor* const ir_visitor_ = nullptr;

   public:
    variable_def_visitor(std::vector<llvm::Value*>* const values,
                         ir_visitor* const ir_visitor)
        : values_{values}, ir_visitor_(ir_visitor) {}
    void visit(details::variable_node& node) override {}
  };
};
}  // namespace ir_visitor
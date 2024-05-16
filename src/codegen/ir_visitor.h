#pragma once

#include <set>
#include <sstream>

#include "llvm/Bitcode/BitcodeWriter.h"
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
inline constexpr std::string Printf = "printf";
inline constexpr std::string PrefixVTable = "__VTable";

struct std_functions_t {
  std::string name;
  std::string return_type;
  std::vector<std::string> params_types;
};

inline std::vector<std_functions_t> std_functions = {
    // class Integer
    {"UnaryMinusI", "Integer", {"Integer"}},
    {"toRealI", "Real", {"Integer"}},
    {"PlusII", "Integer", {"Integer", "Integer"}},
    {"PlusIR", "Real", {"Integer", "Real"}},
    {"MinusII", "Integer", {"Integer", "Integer"}},
    {"MinusIR", "Real", {"Integer", "Real"}},
    {"MultII", "Integer", {"Integer", "Integer"}},
    {"MultIR", "Real", {"Integer", "Real"}},
    {"DivII", "Integer", {"Integer", "Integer"}},
    {"DivIR", "Real", {"Integer", "Real"}},
    {"LessII", "Boolean", {"Integer", "Integer"}},
    {"LessIR", "Boolean", {"Integer", "Real"}},
    {"LessEqII", "Boolean", {"Integer", "Integer"}},
    {"LessEqIR", "Boolean", {"Integer", "Real"}},
    {"GreaterII", "Boolean", {"Integer", "Integer"}},
    {"GreaterIR", "Boolean", {"Integer", "Real"}},
    {"GreaterEqII", "Boolean", {"Integer", "Integer"}},
    {"GreaterEqIR", "Boolean", {"Integer", "Real"}},
    {"EqII", "Boolean", {"Integer", "Integer"}},
    {"EqIR", "Boolean", {"Integer", "Real"}},
    // class Real
    {"UnaryMinusR", "Real", {"Real"}},
    {"toIntegerR", "Integer", {"Real"}},
    {"PlusRR", "Real", {"Real", "Real"}},
    {"PlusRI", "Real", {"Real", "Integer"}},
    {"MinusRR", "Real", {"Real", "Real"}},
    {"MinusRI", "Real", {"Real", "Integer"}},
    {"MultRR", "Real", {"Real", "Real"}},
    {"MultRI", "Real", {"Real", "Integer"}},
    {"DivRR", "Real", {"Real", "Real"}},
    {"DivRI", "Real", {"Real", "Integer"}},
    {"LessRR", "Boolean", {"Real", "Real"}},
    {"LessRI", "Boolean", {"Real", "Integer"}},
    {"LessEqRR", "Boolean", {"Real", "Real"}},
    {"LessEqRI", "Boolean", {"Real", "Integer"}},
    {"GreaterRR", "Boolean", {"Real", "Real"}},
    {"GreaterRI", "Boolean", {"Real", "Integer"}},
    {"GreaterEqRR", "Boolean", {"Real", "Real"}},
    {"GreaterEqRI", "Boolean", {"Real", "Integer"}},
    {"EqRR", "Boolean", {"Real", "Real"}},
    {"EqRI", "Boolean", {"Real", "Integer"}},
    // class Boolean
    {"toIntegerB", "Integer", {"Boolean"}},
    {"OrB", "Boolean", {"Boolean", "Boolean"}},
    {"AndB", "Boolean", {"Boolean", "Boolean"}},
    {"XorB", "Boolean", {"Boolean", "Boolean"}},
    {"NotB", "Boolean", {"Boolean"}},
};

inline void add_metadata(std::unique_ptr<llvm::LLVMContext>& ctx,
                         std::unique_ptr<llvm::IRBuilder<>>& builder,
                         const std::string& name, const std::string& metadata) {
  llvm::Type* i32_type = llvm::IntegerType::getInt32Ty(*ctx);
  llvm::AllocaInst* inst = builder->CreateAlloca(i32_type);
  llvm::MDNode* node = llvm::MDNode::get(*ctx, llvm::MDString::get(*ctx, name));
  inst->setMetadata(metadata, node);
}

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

  std::unordered_map<std::string, llvm::Type*> builtin_types_{
      {"int", llvm::Type::getInt32Ty(*ctx_)},
      {"real", llvm::Type::getFloatTy(*ctx_)},
      {"bool", llvm::Type::getInt1Ty(*ctx_)},
  };

 public:
  void visit(details::program_node& p) override {
    module_->setDataLayout(
        "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
        "S128");

    register_global_funcs(p);
    register_types(p);
    register_members(p);
    register_std_funcs();
    register_vtables(p);
    generate_def_funcs(p);

    std::error_code ec;
    llvm::raw_fd_ostream out_file("out.ll", ec);
    module_->print(out_file, nullptr);
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
      if (builtin_types(name)) continue;

      llvm::StructType::create(*ctx_, llvm::StringRef(name));
      name = vtable_name(c);
      llvm::StructType::create(*ctx_, llvm::StringRef(name));
    }
  }

  void register_members(details::program_node& p) {
    for (auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      if (builtin_types(name)) continue;

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

  void register_std_funcs() {
    for (auto& std_func : std_functions) {
      std::vector<llvm::Type*> params;
      for (const auto& param_type : std_func.params_types) {
        llvm::Type* ptr_cls = get_type_by_name(param_type);
        if (builtin_types(param_type))
          params.push_back(ptr_cls);
        else
          params.push_back(ptr_cls->getPointerTo());
      }
      llvm::Type* ret_type = get_type_by_name(std_func.return_type);

      if (!ret_type) {
        ret_type = llvm::Type::getVoidTy(*ctx_);
      } else if (!builtin_types(std_func.return_type)) {
        ret_type = ret_type->getPointerTo();
      }

      llvm::FunctionType* ptr_func_type =
          llvm::FunctionType::get(ret_type, llvm::ArrayRef(params), false);

      llvm::Function::Create(ptr_func_type,
                             llvm::Function::LinkageTypes::ExternalLinkage,
                             std_func.name, *module_);
    }
  }

  void register_global_funcs(details::program_node& p) {
    module_->getOrInsertFunction(
        Printf,
        llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*ctx_),
                                llvm::Type::getInt8Ty(*ctx_)->getPointerTo(),
                                true /* this is var arg func type*/));

    llvm::Type* ptr_void = llvm::Type::getInt8Ty(*ctx_)->getPointerTo();

    module_->getOrInsertFunction(
        Malloc, llvm::FunctionType::get(
                    ptr_void, llvm::IntegerType::getInt64Ty(*ctx_), false));
  }

  void register_vtables(details::program_node& p) {
    for (const auto& c : p.get_classes()) {
      std::string name = c->get_class_name()->get_identifier()->get_name();
      if (builtin_types(name)) continue;

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
      std::string name = c->get_type()->simple_type();
      if (builtin_types(name)) {
        continue;
      }
      for (const auto& m : c->get_members()) {
        m->visit(&fdv);
      }
    }
  }

  class member_variable_visitor : public visitor::visitor {
   private:
    std::vector<llvm::Type*>* const body_ = nullptr;
    ir_visitor* const ir_visitor_ = nullptr;

   public:
    explicit member_variable_visitor(std::vector<llvm::Type*>* const body,
                                     ir_visitor* ir_visitor)
        : body_{body}, ir_visitor_{ir_visitor} {}

    void visit(details::variable_node& v) override {
      std::string cls_name{
          v.get_type()->get_class_name()->get_identifier()->get_name()};

      llvm::Type* ptrCls = ir_visitor_->get_type_by_name(cls_name);
      body_->push_back(ptrCls);
    }
  };

  class member_method_visitor : public visitor::visitor {
   private:
    const llvm::StructType* const ptr_cls_ = nullptr;
    ir_visitor* const ir_visitor_ = nullptr;

   public:
    explicit member_method_visitor(const llvm::StructType* const ptr_cls,
                                   ir_visitor* ir_visitor)
        : ptr_cls_{ptr_cls}, ir_visitor_{ir_visitor} {}

    void visit(details::method_node& f) override {
      std::vector<llvm::Type*> params;
      params.push_back(ptr_cls_->getPointerTo());
      for (const auto& p : f.get_parameters()->get_parameters()) {
        std::string cls_name = p->get_type()->simple_type();
        llvm::Type* ptr_cls = ir_visitor_->get_type_by_name(cls_name);
        if (ir_visitor_->builtin_types(cls_name))
          params.push_back(ptr_cls);
        else
          params.push_back(ptr_cls->getPointerTo());
      }
      std::string ret_type_name = f.get_return_type()->simple_type();
      llvm::Type* ret_type = ir_visitor_->get_type_by_name(ret_type_name);

      if (!ret_type) {
        ret_type = llvm::Type::getVoidTy(*ir_visitor_->ctx_);
      } else if (!ir_visitor_->builtin_types(ret_type_name)) {
        ret_type = ret_type->getPointerTo();
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
        llvm::Type* ptr_cls = ir_visitor_->get_type_by_name(cls_name);
        if (ir_visitor_->builtin_types(cls_name))
          params.push_back(ptr_cls);
        else
          params.push_back(ptr_cls->getPointerTo());
      }
      const std::string* name =
          c.get_scope()->get_name(scope::scope_type::Class);
      auto ret_type = llvm::StructType::getTypeByName(*ir_visitor_->ctx_,
                                                      llvm::StringRef(*name));

      llvm::FunctionType* ptr_func_type = llvm::FunctionType::get(
          ret_type->getPointerTo(), llvm::ArrayRef(params), false);

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
      types_->push_back(f.get_method_value()->getType());
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

      // generate expr and set return value
      body_visitor bd_visitor(ir_visitor_, cls_to_vars_);
      method.get_body()->visit(&bd_visitor);
      if (!bd_visitor.is_ret_inst()) ir_visitor_->builder_->CreateRetVoid();
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
      ir_visitor_->builder_->CreateRet(ir_visitor_->var_env["this"]);
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
          std::string param_name = "this";
          //          llvm::Type* param_type = param.getType();
          ir_visitor_->var_env[param_name] = &param;
          //          ir_visitor_->builder_->CreateLoad(param_type, &param);

          //          ir_visitor_->builder_->CreateStore(&param,
          //                                             ir_visitor_->var_env[param_name]);
          continue;
        }

        auto param_name = parameters[paramNo - 1]->get_identifier()->get_name();
        param.setName(param_name);
        llvm::Type* param_type = param.getType();
        if (param_type->isPointerTy()) {
          ir_visitor_->var_env[param_name] = &param;
        } else {
          ir_visitor_->var_env[param_name] =
              ir_visitor_->builder_->CreateAlloca(param_type, nullptr,
                                                  llvm::Twine(param_name));
          ir_visitor_->builder_->CreateStore(&param,
                                             ir_visitor_->var_env[param_name]);
        }

        parameters[paramNo - 1]->set_param_value(
            ir_visitor_->var_env[param_name]);
      }
    }
  };

  class body_visitor : public visitor::visitor {
    ir_visitor* const ir_visitor_ = nullptr;
    std::unordered_map<std::string, std::vector<details::variable_node*>>* const
        cls_to_vars_ = nullptr;
    bool is_ret_inst_ = false;

   public:
    explicit body_visitor(
        ir_visitor* const ir_visitor,
        std::unordered_map<std::string,
                           std::vector<details::variable_node*>>* const
            cls_to_vars)
        : ir_visitor_{ir_visitor}, cls_to_vars_{cls_to_vars} {}

    [[nodiscard]] bool is_ret_inst() const noexcept { return is_ret_inst_; }

    void visit(details::variable_node& variable) override {
      variable.get_expression()->visit(this);
      auto bound_val =
          variable.get_expression()->get_final_object()->get_value();

      auto var_name = variable.get_identifier()->get_name();
      //      llvm::Function* parent_function =
      //          ir_visitor_->builder_->GetInsertBlock()->getParent();
      //      llvm::IRBuilder<> tmp_builder(&(parent_function->getEntryBlock()),
      //                                    parent_function->getEntryBlock().begin());
      //      llvm::AllocaInst* var = tmp_builder.CreateAlloca(
      //          bound_val->getType(), nullptr, llvm::Twine(var_name));
      ir_visitor_->var_env[var_name] = bound_val;
      //      ir_visitor_->builder_->CreateStore(bound_val, var);

      variable.set_value(bound_val);
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

    void visit(details::while_loop_node& while_node) override {
      while_node.get_expression()->visit(this);

      auto cond_value =
          while_node.get_expression()->get_final_object()->get_value();
      cond_value = cast_ref_bool_to_primitive_bool(cond_value);

      llvm::Function* parent_function =
          ir_visitor_->builder_->GetInsertBlock()->getParent();
      auto loop_bb = llvm::BasicBlock::Create(*ir_visitor_->ctx_, "loop");
      auto loopend_bb = llvm::BasicBlock::Create(*ir_visitor_->ctx_, "loopend");

      ir_visitor_->builder_->CreateCondBr(cond_value, loop_bb, loopend_bb);
      parent_function->getBasicBlockList().push_back(loop_bb);
      ir_visitor_->builder_->SetInsertPoint(loop_bb);

      while_node.get_body_node()->visit(this);
      while_node.get_expression()->visit(this);

      cond_value = while_node.get_expression()->get_final_object()->get_value();
      cond_value = cast_ref_bool_to_primitive_bool(cond_value);

      loop_bb = ir_visitor_->builder_->GetInsertBlock();
      ir_visitor_->builder_->CreateCondBr(cond_value, loop_bb, loopend_bb);

      parent_function->getBasicBlockList().push_back(loopend_bb);
      ir_visitor_->builder_->SetInsertPoint(loopend_bb);
    }

    void visit(details::if_statement_node& if_node) override {
      if_node.get_expression()->visit(this);
      auto cond_value =
          if_node.get_expression()->get_final_object()->get_value();

      cond_value = cast_ref_bool_to_primitive_bool(cond_value);

      llvm::Function* parent_function =
          ir_visitor_->builder_->GetInsertBlock()->getParent();
      llvm::BasicBlock* then_bb =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "then", parent_function);
      llvm::BasicBlock* else_bb =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "else");
      llvm::BasicBlock* merge_bb =
          llvm::BasicBlock::Create(*ir_visitor_->ctx_, "ifcont");

      ir_visitor_->builder_->CreateCondBr(cond_value, then_bb, else_bb);
      ir_visitor_->builder_->SetInsertPoint(then_bb);
      if_node.get_true_body()->visit(this);
      ir_visitor_->builder_->CreateBr(merge_bb);

      parent_function->getBasicBlockList().push_back(else_bb);
      ir_visitor_->builder_->SetInsertPoint(else_bb);
      if (if_node.get_false_body()) if_node.get_false_body()->visit(this);
      ir_visitor_->builder_->CreateBr(merge_bb);

      parent_function->getBasicBlockList().push_back(merge_bb);
      ir_visitor_->builder_->SetInsertPoint(merge_bb);
    }

    llvm::Value* cast_ref_bool_to_primitive_bool(llvm::Value* value) const {
      if (value->getType()->isPointerTy()) {
        auto type = value->getType()->getPointerElementType();
        // +1 from the table of virtual functions
        value = ir_visitor_->builder_->CreateStructGEP(type, value, 1 + 0);
        type = ir_visitor_->get_type_by_name("bool");
        value = ir_visitor_->builder_->CreateLoad(type, value);
      }
      return value;
    }

    void visit(details::assignment_node& assign) override {
      assign.get_lexpression()->visit(this);
      assign.get_rexpression()->visit(this);

      std::string type_name = assign.get_rexpression()
                                  ->get_final_object()
                                  ->get_type()
                                  ->simple_type();

      //      if (ir_visitor_->builtin_types(type_name)) {
      llvm::Value* val =
          assign.get_rexpression()->get_final_object()->get_value();
      val = ir_visitor_->builder_->CreateLoad(
          val->getType()->getPointerElementType(), val);
      assign.get_rexpression()->get_final_object()->set_value(val);
      //      }

      ir_visitor_->builder_->CreateStore(
          assign.get_rexpression()->get_final_object()->get_value(),
          assign.get_lexpression()->get_final_object()->get_value());
    }

    void visit(details::return_statement_node& return_expr) override {
      return_expr.get_expression()->visit(this);
      std::string ret_name =
          return_expr.get_expression()->get_type()->simple_type();
      auto ret_val =
          return_expr.get_expression()->get_final_object()->get_value();
      if (ret_val->getType()->isVoidTy())
        ir_visitor_->builder_->CreateRetVoid();
      else if (ir_visitor_->builtin_types(ret_name)) {
        ret_val = load_builtin_ret_val(ret_name, ret_val);
        ir_visitor_->builder_->CreateRet(ret_val);
      } else
        ir_visitor_->builder_->CreateRet(ret_val);
      is_ret_inst_ = true;
    }

    llvm::Value* load_builtin_ret_val(const std::string& type_name,
                                      llvm::Value* ret_val) {
      return ir_visitor_->builder_->CreateLoad(
          ir_visitor_->builtin_types_.at(type_name), ret_val);
    }

    void visit(details::member_call& member) override {
      // !if you write `this.i`
      // !its `this`
      member.get_object()->visit(this);
      if (auto method = std::dynamic_pointer_cast<details::method_call>(
              member.get_member());
          method) {
        method->set_owner_value(member.get_object()->get_value());
        member.get_member()->visit(this);
        member.set_value(member.get_member()->get_value());
      } else if (auto variable =
                     std::dynamic_pointer_cast<details::variable_call>(
                         member.get_member())) {
        variable_call_visitor vcv(ir_visitor_, this);
        // !its `i`
        variable->set_owner_value(member.get_object()->get_value());
        member.get_member()->visit(&vcv);
        member.set_value(member.get_member()->get_value());
      }
    }

    void visit(details::variable_call& variable) override {
      if (variable.get_variable() == nullptr) {
        variable.set_value(
            llvm::UndefValue::get(llvm::Type::getVoidTy(*ir_visitor_->ctx_)));
      } else if (auto this_keyword =
                     std::dynamic_pointer_cast<details::this_node>(
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
      } else if (auto literal =
                     std::dynamic_pointer_cast<details::literal_base_node>(
                         variable.get_variable());
                 literal) {
        variable.set_value(create_literal_constant(literal));
      }
    }

    void visit(details::std_call& std_call) override {
      llvm::Function* std_func =
          ir_visitor_->module_->getFunction(std_call.get_method_name());
      std::vector<llvm::Value*> std_func_args;

      for (auto& arg : std_call.get_arguments()) {
        arg->visit(this);
        auto arg_val = arg->get_value();
        if (arg_val == nullptr) {
          std::cout << "error: std_call arg is nullptr\n";
          return;
        }
        std_func_args.push_back(arg_val);
      }
      auto res_val = ir_visitor_->builder_->CreateCall(std_func, std_func_args);
      std_call.set_value(res_val);
    }

    void visit(details::printf_call& printf_call) override {
      llvm::Function* printf = ir_visitor_->module_->getFunction("printf");
      std::vector<llvm::Value*> printf_args;
      printf_args.push_back(ir_visitor_->builder_->CreateGlobalStringPtr(
          printf_call.get_formatted_str()));
      for (auto& arg : printf_call.get_arguments()) {
        arg->visit(this);
        auto arg_val = arg->get_value();
        if (arg_val == nullptr) {
          std::cout << "error: printf arg is nullptr\n";
          return;
        }
        if (arg_val->getType()->isPointerTy()) {
          arg_val = ir_visitor_->builder_->CreateLoad(
              arg_val->getType()->getPointerElementType(), arg_val);
        }
        if (arg_val->getType()->isFloatTy()) {
          arg_val = ir_visitor_->builder_->CreateFPExt(
              arg_val, llvm::Type::getDoubleTy(*ir_visitor_->ctx_));
        }
        printf_args.push_back(arg_val);
      }
      ir_visitor_->builder_->CreateCall(printf, printf_args);
    }

    void visit(details::constructor_call& constr_call) override {
      if (constr_call.get_type()->simple_type() == "base") {
        // handle base call
        return;
      }
      auto obj = create_object(constr_call);

      if (ir_visitor_->builtin_types(constr_call.get_type()->simple_type())) {
        constr_call.set_value(obj);
        return;
      }

      auto callee_fun = constr_call.get_constructor()->get_constr_value();
      if (!callee_fun) {
        std::cout << "Function doesn't exist\n";
        return;
      }

      auto callee_fun_type = callee_fun->getFunctionType();
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

        llvm::Type* param_type = callee_fun_type->getParamType(i + 1);
        if (param_type == nullptr) {
          std::cout << "here\n";
          continue;
        }
        llvm::Value* bit_cast_arg_val =
            ir_visitor_->builder_->CreateBitCast(arg_val, param_type);
        arg_values.push_back(bit_cast_arg_val);
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

      llvm::Type* expected_type = callee_fun_type->getParamType(0);
      llvm::Value* casted_value = ir_visitor_->builder_->CreateBitCast(
          method_call.get_owner_value(), expected_type);

      std::vector<llvm::Value*> arg_values{casted_value};
      auto args = method_call.get_arguments();
      for (size_t i = 0; i < args.size(); i++) {
        args[i]->visit(this);
        llvm::Value* arg_val =
            std::dynamic_pointer_cast<details::expression_ext>(args[i])
                ->get_value();
        if (arg_val == nullptr) {
          std::cout << "null argument when calling function\n";
          return;
        }

        llvm::Type* paramTy = callee_fun_type->getParamType(i + 1);
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
      llvm::Type* type = ir_visitor_->get_type_by_name(type_name);

      if (ir_visitor_->builtin_types(type_name)) {
        if (type_name == "real")
          return llvm::ConstantFP::get(ir_visitor_->get_type_by_name(type_name),
                                       0.0);

        return llvm::ConstantInt::get(ir_visitor_->get_type_by_name(type_name),
                                      0);
        //        llvm::Value* ptr = ir_visitor_->builder_->CreateAlloca(type);
        //
        //        llvm::Value* val = llvm::ConstantInt::get(
        //            llvm::StructType::getInt32Ty(*ir_visitor_->ctx_), 0);
        //
        //        ir_visitor_->builder_->CreateStore(val, ptr);
        //
        //        return ir_visitor_->builder_->CreateLoad(
        //            llvm::StructType::getInt32Ty(*ir_visitor_->ctx_), ptr);
      }

      llvm::Type* int64ty = llvm::Type::getInt64Ty(*ir_visitor_->ctx_);
      llvm::Value* value = llvm::Constant::getNullValue(type->getPointerTo());
      llvm::Value* ptr_obj_size = ir_visitor_->builder_->CreateConstGEP1_64(
          type, value, 1, llvm::Twine("obj_size"));

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
      for (auto& v : cls_to_vars_->at(type_name)) {
        v->get_expression()->visit(this);
        auto val = v->get_expression()->get_value();
        auto type_val = val->getType();
        if (!ir_visitor_->builtin_types(type_name) && type_val->isPointerTy()) {
          type_val = type_val->getPointerElementType();
          val = ir_visitor_->builder_->CreateLoad(type_val, val);
        }
        values.push_back(val);
      }

      for (std::size_t i = 0; i < values.size(); i++) {
        // +1 from the table of virtual functions
        llvm::Value* field =
            ir_visitor_->builder_->CreateStructGEP(type, ptr_obj, 1 + i);

        ir_visitor_->builder_->CreateStore(values[i], field);
      }

      return ptr_obj;
    }

    llvm::Value* create_literal_constant(
        const std::shared_ptr<details::literal_base_node>& literal_base) {
      if (auto literal_int =
              std::dynamic_pointer_cast<details::literal_node<int32_t>>(
                  literal_base);
          literal_int) {
        return llvm::ConstantInt::get(
            llvm::StructType::getInt32Ty(*ir_visitor_->ctx_),
            literal_int->value());
      } else if (auto literal_double =
                     std::dynamic_pointer_cast<details::literal_node<double_t>>(
                         literal_base);
                 literal_double) {
        return llvm::ConstantFP::get(
            llvm::StructType::getFloatTy(*ir_visitor_->ctx_),
            literal_double->value());
      } else if (auto literal_bool =
                     std::dynamic_pointer_cast<details::literal_node<bool>>(
                         literal_base);
                 literal_bool) {
        return llvm::ConstantInt::get(
            llvm::StructType::getInt8Ty(*ir_visitor_->ctx_),
            literal_bool->value());
      }
      std::cout << "ERROR in literal\n";
      return nullptr;
    }
  };

  class variable_call_visitor : public visitor::visitor {
   private:
    ir_visitor* const ir_visitor_ = nullptr;
    body_visitor* const body_visitor_ = nullptr;

   public:
    explicit variable_call_visitor(ir_visitor* const ir_visitor,
                                   body_visitor* const body_visitor)
        : ir_visitor_(ir_visitor), body_visitor_(body_visitor) {}

    void visit(details::expression_node& expression) override {
      expression.visit(body_visitor_);
    }

    void visit(details::body_node& body) override { body.visit(body_visitor_); }

    void visit(details::while_loop_node& while_node) override {
      while_node.visit(body_visitor_);
    }

    void visit(details::if_statement_node& if_node) override {
      if_node.visit(body_visitor_);
    }

    void visit(details::assignment_node& assign) override {
      assign.visit(body_visitor_);
    }

    void visit(details::return_statement_node& return_expr) override {
      return_expr.visit(body_visitor_);
    }

    void visit(details::member_call& member) override {
      member.visit(body_visitor_);
    }

    void visit(details::constructor_call& constr_call) override {
      constr_call.visit(body_visitor_);
    }

    void visit(details::method_call& method_call) override {
      method_call.visit(body_visitor_);
    }

    void visit(details::variable_call& variable) override {
      if (auto var_node = std::dynamic_pointer_cast<details::variable_node>(
              variable.get_variable());
          var_node) {
        llvm::Value* obj_ptr = variable.get_owner();

        llvm::Type* type_ptr = obj_ptr->getType()->getPointerElementType();
        //            llvm::StructType::getTypeByName(*ir_visitor_->ctx_,
        //            type_name);

        // +1 from the table of virtual functions
        llvm::Value* value = ir_visitor_->builder_->CreateStructGEP(
            type_ptr, obj_ptr, 1 + var_node->get_index());

//        add_metadata(ir_visitor_->ctx_, ir_visitor_->builder_, "load value",
//                     "gep");

        variable.set_value(value);
      } else {
        throw std::runtime_error("illegal argument");
      }
    }
  };

  llvm::Type* get_type_by_name(const std::string& name) const {
    if (builtin_types(name)) {
      return builtin_types_.at(name);
    }
    return llvm::StructType::getTypeByName(*ctx_, llvm::StringRef(name));
  }

  [[nodiscard]] bool builtin_types(const std::string& type) const {
    return builtin_types_.contains(type);
  }
};
}  // namespace ir_visitor
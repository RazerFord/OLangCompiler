#include <string>
#include <vector>

#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "semantic-analyzer/visitor.h"
#include "syntactical-analyzer/tree.h"
#include "codegen/ir_visitor.h"

int main(int argc, char** argv) {
  for(int i = 1; i <= 16; ++i) {
    details::type_node::types_.clear();
    std::cout << "TEST: " << i << std::endl;

    auto vec = token_generator::generate_token("./../tests/" + std::to_string(i) + "_test.olg");
    if (vec.empty()) {
      std::cout << "lexical analysis error" << std::endl;
      continue;
    }

    auto ast = tree::make_ast(vec);
    if (ast.fail()) {
      std::cout << "parsing error" << std::endl;
      continue;
    }

    visitor::scope_visitor scope_visitor;
    ast.visit(&scope_visitor);
    scope_visitor.print_error();
    if (scope_visitor.fail()) {
      std::cout << "error on scope checking" << std::endl;
      continue;
    }

    visitor::type_visitor type_visitor;
    ast.visit(&type_visitor);
    type_visitor.print_error();
    if (type_visitor.fail()) {
      std::cout << "error on type checking" << std::endl;
      continue;
    }

    ir_visitor::ir_visitor ir;
    ast.visit(&ir);
  }
}
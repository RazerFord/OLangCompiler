#include <string>
#include <vector>

#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "semantic-analyzer/visitor.h"
#include "syntactical-analyzer/tree.h"
#include "codegen/ir_visitor.h"

int main(int argc, char** argv) {
  for(int i = 1; i <= 1; ++i) {
    details::type_node::types_.clear();
    std::cout << "TEST: " << i << std::endl;

    auto vec = token_generator::generate_token("./../tests/" + std::to_string(i) + "_test.olg", "../std/std.olg");
    auto ast = tree::make_ast(vec);
    if (ast.fail()) {
      continue;
    }

    visitor::scope_visitor scope_visitor;
    ast.visit(&scope_visitor);
    scope_visitor.print_error();
    if (scope_visitor.fail()) {
      continue;
    }

    visitor::type_visitor type_visitor;
    ast.visit(&type_visitor);
    type_visitor.print_error();
    if (type_visitor.fail()) {
      continue;
    }

    ir_visitor::ir_visitor ir;
    ast.visit(&ir);
  }
}
#include <vector>

#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "semantic-analyzer/visitor.h"
#include "syntactical-analyzer/tree.h"

int main(int argc, char** argv) {
  token_generator tokgen;

  auto vec = tokgen.generate_token("./../tests/16_test.olg");

  auto ast = tree::make_ast(vec);
  if (ast.success()) {
    ast.print();
    visitor::scope_visitor scope_visitor;
    ast.visit(&scope_visitor);
    scope_visitor.print_error();

    if (scope_visitor.success()) {
      visitor::type_visitor type_visitor;
      ast.visit(&type_visitor);
      type_visitor.print_error();
    }
  }
  assert(true);
}
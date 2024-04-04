#include <string>
#include <vector>

#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "semantic-analyzer/visitor.h"
#include "syntactical-analyzer/tree.h"

int main(int argc, char** argv) {
  auto vec = token_generator::generate_token("./../tests/16_test.olg");

  auto ast = tree::make_ast(vec);
  if (ast.success()) {
    visitor::scope_visitor scope_visitor;
    ast.visit(&scope_visitor);
    scope_visitor.print_error();

    if (scope_visitor.success()) {
      visitor::type_visitor type_visitor;
      ast.visit(&type_visitor);
      type_visitor.print_error();
    }
  }
}
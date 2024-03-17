#include <vector>

#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "syntactical-analyzer/tree.h"

int main(int argc, char**argv) {
  token_generator tokgen;

  auto vec = tokgen.generate_token("./../tests/15_test.olg");

  auto ast = tree::make_ast(vec);

  assert(true);
}
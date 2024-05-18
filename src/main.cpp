#include <string>

#include "codegen/ir_visitor.h"
#include "lexical-analyzer/token-generator.h"
#include "lexical-analyzer/token.h"
#include "semantic-analyzer/visitor.h"
#include "syntactical-analyzer/tree.h"

int main(int argc, char** argv) {
  if (argc < 2 || strlen(argv[1]) == 0) {
    std::cout << "the path to the program code is required" << std::endl;
    return 1;
  }
  if (argc < 3 || strlen(argv[2]) == 0) {
    std::cout << "output file name required" << std::endl;
    return 1;
  }
  std::string path(argv[1]);
  std::string dest(argv[2]);
  std::cout << "program: " << path << std::endl;
  std::cout << "outfile: " << dest << std::endl;

  details::type_node::types_.clear();
  {
    std::ifstream user_code(path);
    if (!user_code.is_open()) {
      std::cout << "file \"" << path << "\" not found" << std::endl;
      return 1;
    }
  }

  auto vec = token_generator::generate_token(path, "../std/std.olg");
  if (vec.empty()) {
    std::cout << "stage: lexical analysis error" << std::endl;
    return 1;
  }

  auto ast = tree::make_ast(vec);
  if (ast.fail()) {
    std::cout << "stage: parse error" << std::endl;
    return 1;
  }

  visitor::scope_visitor scope_visitor;
  ast.visit(&scope_visitor);
  scope_visitor.print_error();
  if (scope_visitor.fail()) {
    std::cout << "stage: error on scope checking" << std::endl;
    return 1;
  }

  visitor::type_visitor type_visitor;
  ast.visit(&type_visitor);
  type_visitor.print_error();
  if (type_visitor.fail()) {
    std::cout << "stage: error on type checking" << std::endl;
    return 1;
  }

  visitor::indexer_visitor indexer_visitor;
  ast.visit(&indexer_visitor);

  ir_visitor::ir_visitor ir(dest);
  ast.visit(&ir);

  return 0;
}
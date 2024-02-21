#include "lexical-analyzer/token-generator.h"

int main() { 
  for (auto& token: token_generator::generate_token("../tests/10_test.olg")) {
    token->print();
  }
  return 0; 
}
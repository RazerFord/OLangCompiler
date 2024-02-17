#include <fstream>
#include <iostream>
#include <string>

#include "lexical-analyzer/lexical-analyzer.hpp"

int main(int argc, char** argv) {
  std::string file(argv[1]);

  token_generator tg;
  for (const auto& v : tg.generate_token(file)) {
    v->print();
  }
  return 0;
}

#include "Lexer.h"
#include "Parser/Parser.h"

int main() {
  // Your code to use the lexer functionality goes here
  Lexer lexer = Lexer("example.swift");
  std::optional<Token> token;

  Parser parser(lexer);
  return 0;
}
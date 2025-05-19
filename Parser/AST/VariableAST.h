#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include "ExprAST.h"
#include <iostream>
#include <string>

class VariableAST : public ExprAST {
public:
  std::string name;
  VariableAST(const std::string &name) : name(name) {}
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "VariableAST: " << name << std::endl;
  }
};

#endif // VARIABLE_AST_H

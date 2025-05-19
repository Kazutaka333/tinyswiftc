#ifndef INT_AST_H
#define INT_AST_H

#include "ExprAST.h"
#include <iostream>

class IntAST : public ExprAST {
public:
  int value;
  IntAST(int value) : value(value) {}
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "IntAST: " << value << std::endl;
  }
};

#endif // INT_AST_H

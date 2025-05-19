#ifndef RETURN_AST_H
#define RETURN_AST_H

#include "ExprAST.h"
#include <iostream>
#include <memory>

class ReturnAST : public ExprAST {
public:
  std::unique_ptr<ExprAST> expression;
  ReturnAST() = default;
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "ReturnAST:" << std::endl;
    if (expression) {
      expression->print(depth + 1);
    }
  }
};

#endif // RETURN_AST_H

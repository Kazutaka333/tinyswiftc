#ifndef FUNCTION_BODY_AST_H
#define FUNCTION_BODY_AST_H

#include "ASTNode.h"
#include "ExprAST.h"
#include <iostream>
#include <memory>
#include <vector>

class FunctionBodyAST : public ASTNode {
public:
  std::vector<std::unique_ptr<ExprAST>> expressions;
  FunctionBodyAST() = default;
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "FunctionBodyAST:" << std::endl;
    if (!expressions.empty()) {
      for (const auto &expr : expressions) {
        expr->print(depth + 1);
      }
    }
  }
};

#endif // FUNCTION_BODY_AST_H

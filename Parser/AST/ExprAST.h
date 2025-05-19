#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "ASTNode.h"
#include <iostream>

class ExprAST : public ASTNode {
public:
  ExprAST() = default;
  void print(int depth) const override {}
};

#endif // EXPR_AST_H

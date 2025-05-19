#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "ASTNode.h"
#include "FunctionBodyAST.h"
#include "FunctionSignatureAST.h"
#include <iostream>
#include <memory>

class FunctionAST : public ASTNode {
public:
  std::unique_ptr<FunctionSignatureAST> signature;
  std::unique_ptr<FunctionBodyAST> body;
  FunctionAST() = default;
  void print(int depth = 0) const override {
    std::cout << "FunctionAST:" << std::endl;
    if (signature) {
      signature->print(depth + 1);
    }
    if (body) {
      body->print(depth + 1);
    }
  }
};

#endif // FUNCTION_AST_H

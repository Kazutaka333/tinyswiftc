#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "ASTNode.h"
#include "FunctionBodyNode.h"
#include "FunctionSignatureNode.h"
#include <iostream>
#include <memory>

class FunctionNode : public ASTNode {
public:
  std::unique_ptr<FunctionSignatureNode> signature;
  std::unique_ptr<FunctionBodyNode> body;
  FunctionNode() = default;
  void print(int depth = 0) const override {
    std::cout << "FunctionNode:" << std::endl;
    if (signature) {
      signature->print(depth + 1);
    }
    if (body) {
      body->print(depth + 1);
    }
  }
  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // FUNCTION_AST_H

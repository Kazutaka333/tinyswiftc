#ifndef FUNCTION_BODY_AST_H
#define FUNCTION_BODY_AST_H

#include "ASTNode.h"
#include "ExprNode.h"
#include <iostream>
#include <memory>
#include <vector>

class FunctionBodyNode : public ASTNode {
public:
  std::vector<std::unique_ptr<ExprNode>> expressions;
  FunctionBodyNode() = default;
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "FunctionBodyNode:" << std::endl;
    if (!expressions.empty()) {
      for (const auto &expr : expressions) {
        expr->print(depth + 1);
      }
    }
  }
  void codegen(llvm::LLVMContext &context, llvm::Module &module,
               llvm::IRBuilder<> &builder) const override {}
};

#endif // FUNCTION_BODY_AST_H

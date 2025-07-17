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
    debug_log(getBranch(depth), "FunctionBodyNode:");
    if (!expressions.empty()) {
      for (const auto &expr : expressions) {
        expr->print(depth + 1);
      }
    }
  }
  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const {
    for (const auto &expr : expressions) {
      expr->codegen(context, module, builder);
    }
  }
};
#endif // FUNCTION_BODY_AST_H

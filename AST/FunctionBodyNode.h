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
  void print(int Depth) const override {
    debug_log(getBranch(Depth), "FunctionBodyNode:");
    if (!expressions.empty()) {
      for (const auto &expr : expressions) {
        expr->print(Depth + 1);
      }
    }
  }
  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const {
    llvm::Value *last = nullptr;
    for (const auto &expr : expressions) {
      last = expr->codegen(Context, Module, Builder);
    }
    return last;
  }
};
#endif // FUNCTION_BODY_AST_H

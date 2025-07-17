#ifndef EXPR_AST_H
#define EXPR_AST_H

#include "ASTNode.h"
#include <iostream>

class ExprNode : public ASTNode {
public:
  ExprNode() = default;
  virtual llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                               llvm::IRBuilder<> &builder) const = 0;
};

#endif // EXPR_AST_H

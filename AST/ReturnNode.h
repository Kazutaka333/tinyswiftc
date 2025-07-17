#ifndef RETURN_AST_H
#define RETURN_AST_H

#include "ExprNode.h"
#include <iostream>
#include <memory>

class ReturnNode : public ExprNode {
public:
  std::unique_ptr<ExprNode> expression;
  ReturnNode() = default;
  void print(int depth) const override {
    debug_log(getBranch(depth), "ReturnNode: ");
    if (expression) {
      expression->print(depth + 1);
    }
  }

  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const override {

    auto value = expression->codegen(context, module, builder);

    return builder.CreateRet(value); // Return the value from the expression
  }
};

#endif // RETURN_AST_H

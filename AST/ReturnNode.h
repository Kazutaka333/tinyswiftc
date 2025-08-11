#ifndef RETURN_AST_H
#define RETURN_AST_H

#include "ExprNode.h"
#include <iostream>
#include <memory>

class ReturnNode : public ExprNode {
public:
  std::unique_ptr<ExprNode> expression;
  ReturnNode() = default;
  void print(int Depth) const override {
    debug_log(getBranch(Depth), "ReturnNode: ");
    if (expression) {
      expression->print(Depth + 1);
    }
  }

  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const override {

    auto value = expression->codegen(Context, Module, Builder);

    return Builder.CreateRet(value); // Return the value from the expression
  }
};

#endif // RETURN_AST_H

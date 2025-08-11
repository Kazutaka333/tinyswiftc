#ifndef INT_AST_H
#define INT_AST_H

#include "ExprNode.h"
#include <iostream>
#include <llvm/IR/Module.h>

class IntNode : public ExprNode {
public:
  int value;
  IntNode(int value) : value(value) {}

  void print(int Depth) const override {
    debug_log(getBranch(Depth), "IntNode: ", value);
  }

  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const override {

    return Builder.getInt32(value); // Return 0
  }
};

#endif // INT_AST_H

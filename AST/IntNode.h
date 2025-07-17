#ifndef INT_AST_H
#define INT_AST_H

#include "ExprNode.h"
#include <iostream>
#include <llvm/IR/Module.h>

class IntNode : public ExprNode {
public:
  int value;
  IntNode(int value) : value(value) {}

  void print(int depth) const override {
    debug_log(getBranch(depth), "IntNode: ", value);
  }

  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const override {

    return builder.getInt32(value); // Return 0
  }
};

#endif // INT_AST_H

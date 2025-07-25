#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include "ExprNode.h"
#include <iostream>
#include <string>

class VariableNode : public ExprNode {
public:
  std::string name;
  VariableNode(const std::string &name) : name(name) {}
  void print(int depth) const override {
    debug_log(getBranch(depth), "VariableNode: ", name);
  }
  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const override {}
};

#endif // VARIABLE_AST_H

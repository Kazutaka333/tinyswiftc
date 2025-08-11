#ifndef VARIABLE_AST_H
#define VARIABLE_AST_H

#include "ExprNode.h"
#include <iostream>
#include <string>

class VariableNode : public ExprNode {
public:
  std::string name;
  VariableNode(const std::string &name) : name(name) {}
  void print(int Depth) const override {
    debug_log(getBranch(Depth), "VariableNode: ", name);
  }
  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const override {}
};

#endif // VARIABLE_AST_H

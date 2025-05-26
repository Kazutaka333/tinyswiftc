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
    printBranch(depth);
    std::cout << "VariableNode: " << name << std::endl;
  }
  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // VARIABLE_AST_H

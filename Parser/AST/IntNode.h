#ifndef INT_AST_H
#define INT_AST_H

#include "ExprNode.h"
#include <iostream>

class IntNode : public ExprNode {
public:
  int value;
  IntNode(int value) : value(value) {}

  void print(int depth) const override {
    printBranch(depth);
    std::cout << "IntNode: " << value << std::endl;
  }

  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // INT_AST_H

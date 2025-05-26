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
    printBranch(depth);
    std::cout << "ReturnNode:" << std::endl;
    if (expression) {
      expression->print(depth + 1);
    }
  }

  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // RETURN_AST_H

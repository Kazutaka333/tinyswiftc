#ifndef ARGUMENT_AST_H
#define ARGUMENT_AST_H

#include "ASTNode.h"
#include <iostream>
#include <string>

class ArgumentNode : public ASTNode {
  std::string name;
  std::string type;

public:
  ArgumentNode(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
  };
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "ArgumentNode: " << name << " of type " << type << std::endl;
  }
  void codegen(llvm::LLVMContext &context, llvm::Module &module,
               llvm::IRBuilder<> &builder) const override {}
};

#endif // ARGUMENT_AST_H
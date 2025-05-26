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
  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // ARGUMENT_AST_H
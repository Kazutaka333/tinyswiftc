#ifndef ARGUMENT_AST_H
#define ARGUMENT_AST_H

#include "ASTNode.h"
#include <iostream>
#include <sstream>
#include <string>

class ArgumentNode : public ASTNode {
  std::string name;
  std::string type;

public:
  ArgumentNode(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
  };
  void print(int Depth) const override {
    debug_log(getBranch(Depth), "ArgumentNode:", name, "of type", type);
  }
  void codegen(llvm::LLVMContext &Context, llvm::Module &Module,
               llvm::IRBuilder<> &Builder) const {}
};

#endif // ARGUMENT_AST_H
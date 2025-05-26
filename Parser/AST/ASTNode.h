#ifndef ASTNODE_H
#define ASTNODE_H

#include "llvm/IR/LLVMContext.h"
#include <iostream>
#include <memory>

class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual void print(int depth = 0) const = 0;
  virtual void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
                       const std::unique_ptr<llvm::Module> &module) const = 0;
  void printBranch(int depth) const {
    std::cout << std::string(depth * 2, ' ');
    std::cout << "└ ";
  }
};

#endif // ASTNODE_H

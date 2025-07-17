#ifndef ASTNODE_H
#define ASTNODE_H

#include "../Util/debug_log.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <iostream>
#include <memory>

class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual void print(int depth = 0) const = 0;
  std::string getBranch(int depth) const {
    return std::string(depth * 2, ' ') + "└ ";
  }
};

#endif // ASTNODE_H

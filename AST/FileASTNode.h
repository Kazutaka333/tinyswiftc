#ifndef FILE_AST_NODE_H
#define FILE_AST_NODE_H

#include "ASTNode.h"
#include <iostream>
#include <memory>
#include <vector>

class FileASTNode : public ASTNode {

public:
  std::vector<std::unique_ptr<ASTNode>> children;
  FileASTNode() {
    // Default constructor
    this->children = std::vector<std::unique_ptr<ASTNode>>();
  }
  void print(int depth = 0) const override {
    std::cout << "FileASTNode:" << std::endl;
    for (const auto &child : children) {
      child->print();
    }
  }
  void codegen(llvm::LLVMContext &context, llvm::Module &module,
               llvm::IRBuilder<> &builder) const override {
    for (const auto &child : children) {
      if (child) {
        child->codegen(context, module, builder);
      }
    }
  }
};
#endif // FILE_AST_NODE_H

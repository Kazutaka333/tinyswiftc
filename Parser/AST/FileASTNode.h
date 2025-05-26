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
  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};
#endif // FILE_AST_NODE_H

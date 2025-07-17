#ifndef FILE_AST_NODE_H
#define FILE_AST_NODE_H

#include "ASTNode.h"
#include "ExprNode.h"
#include "FunctionBodyNode.h"
#include "FunctionNode.h"
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
    debug_log("FileASTNode:");
    for (const auto &child : children) {
      child->print();
    }
  }
  void *codegen(llvm::LLVMContext &context, llvm::Module &module,
                llvm::IRBuilder<> &builder) const {
    debug_log("Generating code for FileASTNode");
    for (const auto &child : children) {
      if (auto func = dynamic_cast<FunctionNode *>(child.get())) {
        func->codegen(context, module, builder);
        debug_log("Generating code for function: ", func->signature->name);
      } else if (auto func = dynamic_cast<ExprNode *>(child.get())) {
        // func->codegen(context, module, builder);
        debug_log("Generating code for expression");
      } else if (auto func = dynamic_cast<FunctionBodyNode *>(child.get())) {
        func->codegen(context, module, builder);
        debug_log("Generating code for function body");
      }
    }
    // Return a default value, as this is a file node
  }
};
#endif // FILE_AST_NODE_H

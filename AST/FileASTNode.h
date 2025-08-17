#ifndef FILE_AST_NODE_H
#define FILE_AST_NODE_H

#include "ASTNode.h"
#include "ExprNode.h"
#include "FunctionBodyNode.h"
#include "FunctionNode.h"
#include <memory>
#include <vector>

class FileASTNode : public ASTNode {

public:
  std::vector<std::unique_ptr<ASTNode>> Children;
  FileASTNode() {
    // Default constructor
    this->Children = std::vector<std::unique_ptr<ASTNode>>();
  }
  void print(int Depth = 0) const override {
    debug_log("FileASTNode:");
    for (const auto &Child : Children) {
      Child->print();
    }
  }
  void codegen(llvm::LLVMContext &Context, llvm::Module &Module,
               llvm::IRBuilder<> &Builder) const {
    debug_log("Generating code for FileASTNode");
    for (const auto &Child : Children) {
      if (auto Func = dynamic_cast<FunctionNode *>(Child.get())) {
        Func->codegen(Context, Module, Builder);
        debug_log("Generating code for function: ", Func->signature->name);
      } else if (auto Expr = dynamic_cast<ExprNode *>(Child.get())) {
        Expr->codegen(Context, Module, Builder);
        debug_log("Generating code for expression");
      } else if (auto FuncBody =
                     dynamic_cast<FunctionBodyNode *>(Child.get())) {
        FuncBody->codegen(Context, Module, Builder);
        debug_log("Generating code for function body");
      }
    }

    // Return a default value, as this is a file node
  }
};
#endif // FILE_AST_NODE_H

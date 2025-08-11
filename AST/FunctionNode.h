#ifndef FUNCTION_AST_H
#define FUNCTION_AST_H

#include "ASTNode.h"
#include "FunctionBodyNode.h"
#include "FunctionSignatureNode.h"
#include <iostream>
#include <memory>

class FunctionNode : public ASTNode {
public:
  std::unique_ptr<FunctionSignatureNode> signature;
  std::unique_ptr<FunctionBodyNode> body;
  FunctionNode() = default;
  void print(int Depth = 0) const override {
    debug_log(getBranch(Depth), "FunctionNode:");
    if (signature) {
      signature->print(Depth + 1);
    }
    if (body) {
      body->print(Depth + 1);
    }
  }
  void codegen(llvm::LLVMContext &Context, llvm::Module &Module,
               llvm::IRBuilder<> &Builder) const {
    auto func = signature->codegen(Context, Module, Builder);
    // Create entry basic block
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(Context, "", func);

    Builder.SetInsertPoint(entry);
    body->codegen(Context, Module, Builder);
  }
};

#endif // FUNCTION_AST_H

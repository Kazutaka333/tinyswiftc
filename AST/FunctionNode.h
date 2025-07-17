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
  void print(int depth = 0) const override {
    debug_log(getBranch(depth), "FunctionNode:");
    if (signature) {
      signature->print(depth + 1);
    }
    if (body) {
      body->print(depth + 1);
    }
  }
  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const {
    auto func = signature->codegen(context, module, builder);
    // Create entry basic block
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "", func);

    builder.SetInsertPoint(entry);
    body->codegen(context, module, builder);
  }
};

#endif // FUNCTION_AST_H

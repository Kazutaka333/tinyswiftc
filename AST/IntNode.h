#ifndef INT_AST_H
#define INT_AST_H

#include "ExprNode.h"
#include <iostream>
#include <llvm/IR/Module.h>

class IntNode : public ExprNode {
public:
  int value;
  IntNode(int value) : value(value) {}

  void print(int depth) const override {
    printBranch(depth);
    std::cout << "IntNode: " << value << std::endl;
  }

  void codegen(llvm::LLVMContext &context, llvm::Module &module,
               llvm::IRBuilder<> &builder) const override {
    auto mainType = llvm::FunctionType::get(builder.getInt32Ty(), false);
    auto mainFunc = llvm::Function::Create(
        mainType, llvm::Function::ExternalLinkage, "main", module);

    // Create entry basic block
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "", mainFunc);

    builder.CreateRet(llvm::ConstantInt::get(context, llvm::APInt(32, 1)));
  }
};

#endif // INT_AST_H

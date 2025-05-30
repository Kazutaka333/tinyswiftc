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
    // Create function type: int main()
    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);

    // Create function
    llvm::Function *mainFunc = llvm::Function::Create(
        funcType, llvm::Function::ExternalLinkage, "main", module);

    // Create entry basic block
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "", mainFunc);

    builder.SetInsertPoint(entry);
    builder.CreateRet(builder.getInt32(value)); // Return 0
  }
};

#endif // INT_AST_H

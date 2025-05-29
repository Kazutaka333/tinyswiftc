#ifndef FUNCTION_CALL_AST_H
#define FUNCTION_CALL_AST_H

#include "ExprNode.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FunctionCallNode : public ExprNode {
public:
  std::string functionName;
  std::vector<std::unique_ptr<ExprNode>> arguments;
  FunctionCallNode(const std::string &functionName)
      : functionName(functionName) {}
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "FunctionCallNode: " << functionName << std::endl;
    for (const auto &arg : arguments) {
      arg->print(depth + 1);
    }
  }
  void codegen(llvm::LLVMContext &context, llvm::Module &module,
               llvm::IRBuilder<> &builder) const override {}
};

#endif // FUNCTION_CALL_AST_H

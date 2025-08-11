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
  void print(int Depth) const override {
    debug_log(getBranch(Depth), "FunctionCallNode: ", functionName);
    for (const auto &arg : arguments) {
      arg->print(Depth + 1);
    }
  }
  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const override {}
};

#endif // FUNCTION_CALL_AST_H

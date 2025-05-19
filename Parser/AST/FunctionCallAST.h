#ifndef FUNCTION_CALL_AST_H
#define FUNCTION_CALL_AST_H

#include "ExprAST.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FunctionCallAST : public ExprAST {
public:
  std::string functionName;
  std::vector<std::unique_ptr<ExprAST>> arguments;
  FunctionCallAST(const std::string &functionName)
      : functionName(functionName) {}
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "FunctionCallAST: " << functionName << std::endl;
    for (const auto &arg : arguments) {
      arg->print(depth + 1);
    }
  }
};

#endif // FUNCTION_CALL_AST_H

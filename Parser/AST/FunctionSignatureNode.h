#ifndef FUNCTION_SIGNATURE_AST_H
#define FUNCTION_SIGNATURE_AST_H

#include "ASTNode.h"
#include "ArgumentNode.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FunctionSignatureNode : public ASTNode {
public:
  std::string name;
  std::vector<std::unique_ptr<ArgumentNode>> parameters;
  std::string returnType;
  FunctionSignatureNode() {
    // Default constructor
    this->name = "";
    this->returnType = "";
    this->parameters = std::vector<std::unique_ptr<ArgumentNode>>();
  };
  void print(int depth) const override {
    // show the same number of hyphen as the depth
    printBranch(depth);
    std::cout << "FunctionSignatureNode: \"" << name << "\" with return type \""
              << returnType << "\"" << std::endl;
    for (const auto &param : parameters) {
      param->print(depth + 1);
    }
  }
  void codegen(const std::unique_ptr<llvm::LLVMContext> &context,
               const std::unique_ptr<llvm::Module> &module) const override {}
};

#endif // FUNCTION_SIGNATURE_AST_H

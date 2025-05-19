#ifndef FUNCTION_SIGNATURE_AST_H
#define FUNCTION_SIGNATURE_AST_H

#include "ASTNode.h"
#include "ArgumentAST.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FunctionSignatureAST : public ASTNode {
public:
  std::string name;
  std::vector<std::unique_ptr<ArgumentAST>> parameters;
  std::string returnType;
  FunctionSignatureAST() {
    // Default constructor
    this->name = "";
    this->returnType = "";
    this->parameters = std::vector<std::unique_ptr<ArgumentAST>>();
  };
  void print(int depth) const override {
    // show the same number of hyphen as the depth
    printBranch(depth);
    std::cout << "FunctionSignatureAST: \"" << name << "\" with return type \""
              << returnType << "\"" << std::endl;
    for (const auto &param : parameters) {
      param->print(depth + 1);
    }
  }
};

#endif // FUNCTION_SIGNATURE_AST_H

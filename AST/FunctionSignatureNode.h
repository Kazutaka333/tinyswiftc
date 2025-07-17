#ifndef FUNCTION_SIGNATURE_AST_H
#define FUNCTION_SIGNATURE_AST_H

#include "ASTNode.h"
#include "ArgumentNode.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class FunctionSignatureNode {
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
  std::string getBranch(int depth) const {
    return std::string(depth * 2, ' ') + "└ ";
  }
  void print(int depth) const {
    // show the same number of hyphen as the depth
    debug_log(getBranch(depth), "FunctionSignatureNode: \"", name,
              "\" with return type \"", returnType, "\"");
    for (const auto &param : parameters) {
      param->print(depth + 1);
    }
  }
  llvm::Function *codegen(llvm::LLVMContext &context, llvm::Module &module,
                          llvm::IRBuilder<> &builder) const {
    if (returnType != "Int") {
      std::cerr << "Unsupported return type: " << returnType << std::endl;
      return nullptr;
    }
    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);

    // Create function
    return llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                                  "main", module);
  }
};

#endif // FUNCTION_SIGNATURE_AST_H

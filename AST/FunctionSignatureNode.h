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
  std::string getBranch(int Depth) const {
    return std::string(Depth * 2, ' ') + "└ ";
  }
  void print(int Depth) const {
    // show the same number of hyphen as the Depth
    debug_log(getBranch(Depth), "FunctionSignatureNode: \"", name,
              "\" with return type \"", returnType, "\"");
    for (const auto &param : parameters) {
      param->print(Depth + 1);
    }
  }
  llvm::Function *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                          llvm::IRBuilder<> &Builder) const {
    if (returnType != "Int") {
      std::cerr << "Unsupported return type: " << returnType << std::endl;
      return nullptr;
    }
    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), false);

    // Create function
    return llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,
                                  name, Module);
  }
};

#endif // FUNCTION_SIGNATURE_AST_H

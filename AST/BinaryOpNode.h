#include "ExprNode.h"
#include <iostream>
#include <sstream>
class BinaryOpNode : public ExprNode {
public:
  std::unique_ptr<ExprNode> Left;
  std::unique_ptr<ExprNode> Right;
  std::string OP;
  std::string Type;
  int ParenthesisCount;
  BinaryOpNode(std::string OP, std::string Type, int ParenthesisCount)
      : OP{OP}, Type{Type}, ParenthesisCount{ParenthesisCount} {};

  void print(int Depth) const override {
    debug_log(getBranch(Depth), " BinaryOpNode: ", OP,
              ", parenthesisCount: ", ParenthesisCount);
    if (Left) {
      Left->print(Depth + 1);
    }
    if (Right) {
      Right->print(Depth + 1);
    }
  }

  llvm::Value *codegen(llvm::LLVMContext &Context, llvm::Module &Module,
                       llvm::IRBuilder<> &Builder) const override {
    llvm::Value *LeftVal = Left->codegen(Context, Module, Builder);
    llvm::Value *RightVal = Right->codegen(Context, Module, Builder);
    if (OP == "+") {
      return Builder.CreateAdd(LeftVal, RightVal);
    } else if (OP == "-") {
      return Builder.CreateSub(LeftVal, RightVal);
    } else if (OP == "*") {
      return Builder.CreateMul(LeftVal, RightVal);
    } else if (OP == "/") {
      return Builder.CreateSDiv(LeftVal, RightVal);
    } else {
      std::cerr << "Unknown Binary Operator " << OP << std::endl;
    }
  }
};
#include "ExprNode.h"
#include <sstream>
class BinaryOpNode : public ExprNode {
public:
  std::unique_ptr<ExprNode> left;
  std::unique_ptr<ExprNode> right;
  std::string op;
  BinaryOpNode() = default;

  void print(int depth) const override {
    debug_log(getBranch(depth), "BinaryOpNode: ", op);
    if (left) {
      left->print(depth + 1);
    }
    if (right) {
      right->print(depth + 1);
    }
  }

  llvm::Value *codegen(llvm::LLVMContext &context, llvm::Module &module,
                       llvm::IRBuilder<> &builder) const override {

    return builder.getInt32(0); // Return 0
  }
};
#ifndef ARGUMENT_AST_H
#define ARGUMENT_AST_H

#include "ASTNode.h"
#include <iostream>
#include <string>

class ArgumentAST : public ASTNode {
  std::string name;
  std::string type;

public:
  ArgumentAST(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
  };
  void print(int depth) const override {
    printBranch(depth);
    std::cout << "ArgumentAST: " << name << " of type " << type << std::endl;
  }
};

#endif // ARGUMENT_AST_H
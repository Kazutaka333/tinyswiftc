#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>

class ASTNode {
public:
  virtual ~ASTNode() = default;
  virtual void print(int depth = 0) const = 0;
  void printBranch(int depth) const {
    std::cout << std::string(depth * 2, ' ');
    std::cout << "└ ";
  }
};

#endif // ASTNODE_H

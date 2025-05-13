#ifndef PARSER_H
#define PARSER_H

#include "../Lexer.h"
#include <iostream>
#include <memory>

class ASTNode {
public:
  int depth = 0;
  virtual ~ASTNode() {};
  virtual void print() const = 0;
  void printBranch() const {
    std::cout << std::string(depth * 2, ' ');
    std::cout << "└ ";
  }
};

class FileASTNode : public ASTNode {

public:
  std::vector<std::unique_ptr<ASTNode>> children;
  FileASTNode() {
    // Default constructor
    this->children = std::vector<std::unique_ptr<ASTNode>>();
  }
  void print() const override {
    std::cout << "FileASTNode:" << std::endl;
    for (const auto &child : children) {
      child->print();
    }
  }
};
class ArgumentAST : public ASTNode {
  std::string name;
  std::string type;

public:
  ArgumentAST(const std::string &name, const std::string &type) {
    this->name = name;
    this->type = type;
  };
  void print() const override {
    // Example implementation for demonstration
    printBranch();
    std::cout << "ArgumentAST: " << name << " of type " << type << std::endl;
  }
};

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
  void print() const override {
    // show the same number of hyphen as the depth
    printBranch();
    std::cout << "FunctionSignatureAST: \"" << name << "\" with return type \""
              << returnType << "\"" << std::endl;
    for (const auto &param : parameters) {
      param->print();
    }
  }
};

class ExprAST : public ASTNode {
public:
  ExprAST() = default;
  void print() const override {}
};

class IntAST : public ExprAST {
public:
  int value;
  IntAST(int value) : value(value) {}
  void print() const override {
    printBranch();
    std::cout << "IntAST: " << value << std::endl;
  }
};

class FunctionCallAST : public ExprAST {
public:
  std::string functionName;
  std::vector<std::unique_ptr<ExprAST>> arguments;
  FunctionCallAST(const std::string &functionName)
      : functionName(functionName) {}
  void print() const override {
    printBranch();
    std::cout << "FunctionCallAST: " << functionName << std::endl;
    for (const auto &arg : arguments) {
      arg->print();
    }
  }
};

class VariableAST : public ExprAST {
public:
  std::string name;
  VariableAST(const std::string &name) : name(name) {}
  void print() const override {
    printBranch();
    std::cout << "VariableAST: " << name << std::endl;
  }
};

class ReturnAST : public ExprAST {
public:
  std::unique_ptr<ExprAST> expression;
  ReturnAST() = default;
  void print() const override {
    printBranch();
    std::cout << "ReturnAST:" << std::endl;
    if (expression) {
      expression->print();
    }
  }
};

class FunctionBodyAST : public ASTNode {
public:
  std::vector<std::unique_ptr<ExprAST>> expressions;
  FunctionBodyAST() = default;
  void print() const override {
    printBranch();
    std::cout << "FunctionBodyAST:" << std::endl;
    if (!expressions.empty()) {
      for (const auto &expr : expressions) {
        expr->print();
      }
    }
  }
};

class FunctionAST : public ASTNode {
public:
  std::unique_ptr<FunctionSignatureAST> signature;
  std::unique_ptr<FunctionBodyAST> body;
  FunctionAST() = default;
  void print() const override {
    std::cout << "FunctionAST:" << std::endl;
    if (signature) {
      signature->print();
    }
    if (body) {
      body->print();
    }
  }
};

class Parser {
  FileASTNode root;
  Lexer &lexer;
  Token currentToken;
  void parseFunctionSignature(FunctionAST &func);
  void parseParameterList(FunctionSignatureAST &funcSignature);
  void parseFunctionBody(FunctionAST &func);

public:
  Parser(Lexer &lexer);
  void printAST() const;
};

#endif
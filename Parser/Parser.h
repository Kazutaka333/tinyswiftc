#ifndef PARSER_H
#define PARSER_H

#include "../Lexer.h"
#include "AST/ASTNode.h"
#include "AST/ExprAST.h"
#include "AST/FileASTNode.h"
#include "AST/FunctionAST.h"
#include "AST/FunctionBodyAST.h"
#include "AST/FunctionCallAST.h"
#include "AST/FunctionSignatureAST.h" // Include the new header file
#include "AST/IntAST.h"
#include "AST/ReturnAST.h"
#include "AST/VariableAST.h"
#include <iostream>
#include <memory>

class Parser {
  FileASTNode root;
  Lexer &lexer;
  Token currentToken;
  std::unique_ptr<FunctionSignatureAST> parseFunctionSignature();
  std::vector<std::unique_ptr<ArgumentAST>> parseParameterList();
  std::unique_ptr<FunctionBodyAST> parseFunctionBody();
  std::unique_ptr<ExprAST> parseExpression();

public:
  Parser(Lexer &lexer);
  void printAST() const;
};

#endif
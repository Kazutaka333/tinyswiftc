#ifndef PARSER_H
#define PARSER_H

#include "../AST/ASTNode.h"
#include "../AST/ExprNode.h"
#include "../AST/FileASTNode.h"
#include "../AST/FunctionBodyNode.h"
#include "../AST/FunctionCallNode.h"
#include "../AST/FunctionNode.h"
#include "../AST/FunctionSignatureNode.h" // Include the new header file
#include "../AST/IntNode.h"
#include "../AST/ReturnNode.h"
#include "../AST/VariableNode.h"
#include "../Lexer/Lexer.h"
#include <iostream>
#include <memory>

class Parser {
  Lexer &lexer;
  Token currentToken;
  std::unique_ptr<FunctionSignatureNode> parseFunctionSignature();
  std::vector<std::unique_ptr<ArgumentNode>> parseParameterList();
  std::unique_ptr<ArgumentNode> parseArgument();
  std::unique_ptr<FunctionBodyNode> parseFunctionBody();
  std::unique_ptr<ReturnNode> parseReturnExpression();
  std::unique_ptr<ExprNode> parseExpression();

public:
  std::unique_ptr<FileASTNode> root;
  Parser(Lexer &lexer);
};

#endif
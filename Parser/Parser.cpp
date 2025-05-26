#include "Parser.h"
#include "../Lexer/Lexer.h"
#include "AST/ArgumentNode.h"
#include "AST/FileASTNode.h"
#include "AST/FunctionSignatureNode.h"
#include "AST/ReturnNode.h"
#include <cstddef>
#include <memory>
#include <vector>

Parser::Parser(Lexer &lexer)
    : lexer(lexer), root(std::make_unique<FileASTNode>()) {
  // Initialize the parser with the lexer
  while (true) {
    currentToken = lexer.getNextToken();
    if (currentToken.type == tok_eof) {
      break;
    }
    if (currentToken.type == tok_func) {
      auto func = std::make_unique<FunctionNode>();
      func->signature = parseFunctionSignature();
      func->body = parseFunctionBody();

      root->children.push_back(std::move(func));
    }
  }

  root->print();
}

std::vector<std::unique_ptr<ArgumentNode>> Parser::parseParameterList() {
  auto params = std::vector<std::unique_ptr<ArgumentNode>>();
  while (true) {
    auto param = parseArgument();
    params.push_back(std::move(param));
    if (currentToken.type == tok_comma) {
      currentToken = lexer.getNextToken(); // Consume ','
    } else if (currentToken.type == tok_right_paren) {
      break; // End of parameters
    } else {
      std::cerr << "Expected ',' or ')'" << std::endl;
      return params;
    }
  }
  currentToken = lexer.getNextToken(); // Consume ')'
  return params;
}

std::unique_ptr<ArgumentNode> Parser::parseArgument() {
  if (currentToken.type != tok_identifier) {
    std::cerr << "Expected parameter name" << std::endl;
    return nullptr;
  }
  std::string paramName = currentToken.identifierName;
  currentToken = lexer.getNextToken(); // Consume parameter name
  if (currentToken.type != tok_colon) {
    std::cerr << "Expected ':' after parameter name" << std::endl;
    return nullptr;
  }
  currentToken = lexer.getNextToken(); // Consume ':'
  if (currentToken.type != tok_identifier) {
    std::cerr << "Expected parameter type" << std::endl;
    return nullptr;
  }
  std::string paramType = currentToken.identifierName;
  auto param = std::make_unique<ArgumentNode>(paramName, paramType);
  currentToken = lexer.getNextToken(); // Consume parameter type
  return std::move(param);
}

std::unique_ptr<FunctionSignatureNode> Parser::parseFunctionSignature() {
  std::cout << "Parsing function signature..." << std::endl;
  auto funcSignature = std::make_unique<FunctionSignatureNode>();

  std::string funcName;
  std::string returnType;

  currentToken = lexer.getNextToken(); // Consume the function keyword
  if (currentToken.type != tok_identifier) {
    std::cerr << "Expected function name after 'func'" << std::endl;
    return nullptr;
  }
  funcName = currentToken.identifierName;
  currentToken = lexer.getNextToken(); // Consume the function name
  if (currentToken.type != tok_left_paren) {
    std::cerr << "Expected '(' after function name" << std::endl;
    return nullptr;
  }
  currentToken = lexer.getNextToken(); // Consume '('
  if (currentToken.type == tok_right_paren) {
    // No parameters
    funcSignature->parameters = std::vector<std::unique_ptr<ArgumentNode>>();
  } else {
    funcSignature->parameters = parseParameterList();
  }

  if (currentToken.type == tok_arrow) {
    currentToken = lexer.getNextToken(); // Consume '->'
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected return type after '->'" << std::endl;
      return nullptr;
    }
    returnType = currentToken.identifierName;
    currentToken = lexer.getNextToken(); // Consume return type
  }

  funcSignature->name = funcName;
  funcSignature->returnType = returnType;

  return std::move(funcSignature);
}

std::unique_ptr<FunctionBodyNode> Parser::parseFunctionBody() {
  auto funcBody = std::make_unique<FunctionBodyNode>();

  std::cout << "Parsing function body..." << std::endl;
  if (currentToken.type != tok_left_brace) {
    std::cerr << "Expected '{' to start function body" << std::endl;
    return nullptr;
  }
  currentToken = lexer.getNextToken(); // Consume '{'

  while (currentToken.type != tok_right_brace) {
    if (currentToken.type == tok_eof) {
      std::cerr << "Unexpected end of file in function body" << std::endl;
      return nullptr;
    }
    if (currentToken.type == tok_return) {
      auto returnExpr = parseReturnExpression();
      funcBody->expressions.push_back(std::move(returnExpr));
    } else {
      std::cerr << "Expected expression in function body but found:"
                << currentToken.type << std::endl;

      return nullptr;
    }
    currentToken = lexer.getNextToken(); // Consume expression
  }

  if (currentToken.type != tok_right_brace) {
    std::cerr << "Expected '}' to end function body" << std::endl;
    return nullptr;
  }
  currentToken = lexer.getNextToken(); // Consume '}'
  return std::move(funcBody);
}

std::unique_ptr<ReturnNode> Parser::parseReturnExpression() {
  currentToken = lexer.getNextToken(); // Consume 'return'
  auto returnExpr = std::make_unique<ReturnNode>();

  std::unique_ptr<ExprNode> expr = parseExpression();
  if (!expr) {
    std::cerr << "Expected expression after 'return'" << std::endl;
    return nullptr;
  }

  returnExpr->expression = std::move(expr);
  return std::move(returnExpr);
}

std::unique_ptr<ExprNode> Parser::parseExpression() {
  std::unique_ptr<ExprNode> expr;
  if (currentToken.type == tok_identifier) {
    expr = std::make_unique<VariableNode>(currentToken.identifierName);
  } else if (currentToken.type == tok_int) {
    expr = std::make_unique<IntNode>(currentToken.intValue);
  } else {
    std::cerr << "Expected expression after 'return' but found: "
              << currentToken.type << std::endl;
    return nullptr;
  }
  return std::move(expr);
}
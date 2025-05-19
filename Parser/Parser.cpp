#include "Parser.h"
#include "../Lexer.h"
#include "AST/ArgumentAST.h"
#include "AST/FileASTNode.h"
#include "AST/FunctionSignatureAST.h"
#include <cstddef>
#include <memory>
#include <vector>

Parser::Parser(Lexer &lexer) : lexer(lexer), root(FileASTNode()) {
  // Initialize the parser with the lexer
  while (true) {
    currentToken = lexer.getNextToken();
    if (currentToken.type == tok_eof) {
      break;
    }
    if (currentToken.type == tok_func) {
      auto func = std::make_unique<FunctionAST>();
      func->signature = parseFunctionSignature();
      func->body = parseFunctionBody();
      root.children.push_back(std::move(func));
    }
  }

  root.print();
}

std::vector<std::unique_ptr<ArgumentAST>> Parser::parseParameterList() {
  auto params = std::vector<std::unique_ptr<ArgumentAST>>();
  while (true) {
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected parameter name" << std::endl;
      return params;
    }
    std::string paramName = currentToken.identifierName;
    currentToken = lexer.getNextToken(); // Consume parameter name
    if (currentToken.type != tok_colon) {
      std::cerr << "Expected ':' after parameter name" << std::endl;
      return params;
    }
    currentToken = lexer.getNextToken(); // Consume ':'
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected parameter type" << std::endl;
      return params;
    }
    std::string paramType = currentToken.identifierName;
    auto param = std::make_unique<ArgumentAST>(paramName, paramType);
    params.push_back(std::move(param));
    currentToken = lexer.getNextToken(); // Consume parameter type
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

std::unique_ptr<FunctionSignatureAST> Parser::parseFunctionSignature() {
  std::cout << "Parsing function signature..." << std::endl;
  auto funcSignature = std::make_unique<FunctionSignatureAST>();

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
    funcSignature->parameters = std::vector<std::unique_ptr<ArgumentAST>>();
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

std::unique_ptr<FunctionBodyAST> Parser::parseFunctionBody() {
  auto funcBody = std::make_unique<FunctionBodyAST>();

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
      currentToken = lexer.getNextToken(); // Consume 'return'
      auto returnExpr = std::make_unique<ReturnAST>();

      std::unique_ptr<ExprAST> expr = parseExpression();
      if (!expr) {
        std::cerr << "Expected expression after 'return'" << std::endl;
        return nullptr;
      }

      returnExpr->expression = std::move(expr);
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

std::unique_ptr<ExprAST> Parser::parseExpression() {
  std::unique_ptr<ExprAST> expr;
  if (currentToken.type == tok_identifier) {
    expr = std::make_unique<VariableAST>(currentToken.identifierName);
  } else if (currentToken.type == tok_int) {
    expr = std::make_unique<IntAST>(currentToken.intValue);
  } else {
    std::cerr << "Expected expression after 'return' but found: "
              << currentToken.type << std::endl;
    return nullptr;
  }
  return std::move(expr);
}
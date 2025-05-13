#include "Parser.h"
#include "../Lexer.h"
#include <memory>

Parser::Parser(Lexer &lexer) : lexer(lexer), root(FileASTNode()) {
  // Initialize the parser with the lexer
  while (true) {
    currentToken = lexer.getNextToken();
    if (currentToken.type == tok_eof) {
      break;
    }
    if (currentToken.type == tok_func) {
      auto func = std::make_unique<FunctionAST>();
      parseFunctionSignature(*func);
      parseFunctionBody(*func);
      root.children.push_back(std::move(func));
    }
  }

  root.print();
}

void Parser::parseParameterList(FunctionSignatureAST &funcSignature) {
  while (true) {
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected parameter name" << std::endl;
      return;
    }
    std::string paramName = currentToken.identifierName;
    currentToken = lexer.getNextToken(); // Consume parameter name
    if (currentToken.type != tok_colon) {
      std::cerr << "Expected ':' after parameter name" << std::endl;
      return;
    }
    currentToken = lexer.getNextToken(); // Consume ':'
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected parameter type" << std::endl;
      return;
    }
    std::string paramType = currentToken.identifierName;
    auto param = std::make_unique<ArgumentAST>(paramName, paramType);
    param->depth = funcSignature.depth + 1;
    funcSignature.parameters.push_back(std::move(param));
    currentToken = lexer.getNextToken(); // Consume parameter type
    if (currentToken.type == tok_comma) {
      currentToken = lexer.getNextToken(); // Consume ','
    } else if (currentToken.type == tok_right_paren) {
      break; // End of parameters
    } else {
      std::cerr << "Expected ',' or ')'" << std::endl;
      return;
    }
  }
  currentToken = lexer.getNextToken(); // Consume ')'
}

void Parser::parseFunctionSignature(FunctionAST &func) {
  std::cout << "Parsing function signature..." << std::endl;
  auto funcSignature = std::make_unique<FunctionSignatureAST>();
  funcSignature->depth = func.depth + 1;

  std::string funcName;
  std::string returnType;

  currentToken = lexer.getNextToken(); // Consume the function keyword
  if (currentToken.type != tok_identifier) {
    std::cerr << "Expected function name after 'func'" << std::endl;
    return;
  }
  funcName = currentToken.identifierName;
  currentToken = lexer.getNextToken(); // Consume the function name
  if (currentToken.type != tok_left_paren) {
    std::cerr << "Expected '(' after function name" << std::endl;
    return;
  }
  currentToken = lexer.getNextToken(); // Consume '('
  if (currentToken.type == tok_right_paren) {
    // No parameters
  } else {
    parseParameterList(*funcSignature);
  }

  if (currentToken.type == tok_arrow) {
    currentToken = lexer.getNextToken(); // Consume '->'
    if (currentToken.type != tok_identifier) {
      std::cerr << "Expected return type after '->'" << std::endl;
      return;
    }
    returnType = currentToken.identifierName;
    currentToken = lexer.getNextToken(); // Consume return type
  }

  funcSignature->name = funcName;
  funcSignature->returnType = returnType;

  func.signature = std::move(funcSignature);
}

void Parser::parseFunctionBody(FunctionAST &func) {
  auto funcBody = std::make_unique<FunctionBodyAST>();
  funcBody->depth = func.depth + 1;
  std::cout << "Parsing function body..." << std::endl;
  if (currentToken.type != tok_left_brace) {
    std::cerr << "Expected '{' to start function body" << std::endl;
    return;
  }
  currentToken = lexer.getNextToken(); // Consume '{'

  while (currentToken.type != tok_right_brace) {
    if (currentToken.type == tok_eof) {
      std::cerr << "Unexpected end of file in function body" << std::endl;
      return;
    }
    if (currentToken.type == tok_return) {
      currentToken = lexer.getNextToken(); // Consume 'return'
      auto returnExpr = std::make_unique<ReturnAST>();
      returnExpr->depth = funcBody->depth + 1;
      std::unique_ptr<ExprAST> expr;
      if (currentToken.type == tok_identifier) {
        expr = std::make_unique<VariableAST>(currentToken.identifierName);
        expr->depth = returnExpr->depth + 1;
      } else if (currentToken.type == tok_int) {
        expr = std::make_unique<IntAST>(currentToken.intValue);
        expr->depth = returnExpr->depth + 1;
      } else {
        std::cerr << "Expected expression after 'return' but found: "
                  << currentToken.type << std::endl;
        return;
      }
      returnExpr->expression = std::move(expr);
      funcBody->expressions.push_back(std::move(returnExpr));
    } else {
      std::cerr << "Expected expression in function body but found:"
                << currentToken.type << std::endl;

      return;
    }
    currentToken = lexer.getNextToken(); // Consume expression
  }

  if (currentToken.type != tok_right_brace) {
    std::cerr << "Expected '}' to end function body" << std::endl;
    return;
  }
  func.body = std::move(funcBody);

  currentToken = lexer.getNextToken(); // Consume '}'
}
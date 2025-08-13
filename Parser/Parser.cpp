#include "Parser.h"
#include "../AST/ArgumentNode.h"
#include "../AST/FileASTNode.h"
#include "../AST/FunctionSignatureNode.h"
#include "../AST/ReturnNode.h"
#include "../Lexer/Lexer.h"
#include <cstddef>
#include <map>
#include <memory>
#include <utility>
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

static std::map<std::string, int> PrecedenceTable = {
    {"*", 1},
    {"/", 1},
    {"+", 0},
    {"-", 0},
};

std::vector<std::unique_ptr<ArgumentNode>> Parser::parseParameterList() {
  auto params = std::vector<std::unique_ptr<ArgumentNode>>();
  while (currentToken.type != tok_right_paren) {
    auto param = parseArgument();
    if (param) {
      params.push_back(std::move(param));
    }
    if (currentToken.type == tok_comma) {
      currentToken = lexer.getNextToken(); // Consume ','
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
  funcSignature->parameters = parseParameterList();

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

  if (currentToken.type != tok_left_brace) {
    std::cerr << "Expected '{' to start function body but found "
              << currentToken.type << std::endl;
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
                << currentToken.type << " " << currentToken.identifierName
                << std::endl;

      return nullptr;
    }
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

  std::unique_ptr<ExprNode> expr = parseExpression(0);
  if (!expr) {
    std::cerr << "Expected expression after 'return'" << std::endl;
    return nullptr;
  }

  returnExpr->expression = std::move(expr);
  return std::move(returnExpr);
}

std::unique_ptr<ExprNode> Parser::parseExpression(int parenthesisCount) {
  std::unique_ptr<ExprNode> expr;
  while (currentToken.type == tok_left_paren) {
    currentToken = lexer.getNextToken(); // Consume left parenthesis
    parenthesisCount++;
  }

  if (currentToken.type == tok_identifier) {
    expr = std::make_unique<VariableNode>(currentToken.identifierName);
  } else if (currentToken.type == tok_int) {
    expr = std::make_unique<IntNode>(currentToken.intValue);
  } else {
    std::cerr << "Failed to parse expression: found " << currentToken.type
              << std::endl;
    return nullptr;
  }
  currentToken = lexer.getNextToken(); // Consume expression or Left operand
                                       // parse Binary Operator
  while (currentToken.type == tok_right_paren) {
    currentToken = lexer.getNextToken(); // Consume right parenthesis
    parenthesisCount--;
  }
  if (currentToken.type == tok_plus || currentToken.type == tok_minus ||
      currentToken.type == tok_multiply || currentToken.type == tok_divide) {
    auto binOpNode = parseRightBinaryOpExpression(parenthesisCount);
    BinaryOpNode *tempNode = binOpNode.get();
    while (auto tempBinNode =
               dynamic_cast<BinaryOpNode *>(tempNode->Left.get())) {
      tempNode = tempBinNode;
    }
    tempNode->Left = std::move(expr);
    return std::move(binOpNode);
  }

  return std::move(expr);
}

std::unique_ptr<BinaryOpNode>
Parser::parseRightBinaryOpExpression(int parenthesisCount) {
  auto binOpNode = std::make_unique<BinaryOpNode>(parenthesisCount);
  binOpNode->OP = currentToken.identifierName;
  currentToken = lexer.getNextToken(); // Consume the operator
  auto rightExpr = parseExpression(parenthesisCount);
  if (auto rightBinOpNode = dynamic_cast<BinaryOpNode *>(rightExpr.get())) {
    bool leftOpHasMoreParenthese =
        binOpNode->parenthesisCount > rightBinOpNode->parenthesisCount;
    bool leftAndRightOpHaveSameParenthese =
        binOpNode->parenthesisCount == rightBinOpNode->parenthesisCount;
    bool leftOpHasHigherOrEqaulPrecendence =
        PrecedenceTable[binOpNode->OP] >= PrecedenceTable[rightBinOpNode->OP];
    bool leftOpIsPrioritized =
        leftOpHasMoreParenthese ||
        (leftAndRightOpHaveSameParenthese && leftOpHasHigherOrEqaulPrecendence);
    // current op is more prioritized than right op
    if (leftOpIsPrioritized) {
      binOpNode->Right = std::move(rightBinOpNode->Left);
      rightBinOpNode->Left = std::move(binOpNode);
      auto unused = rightExpr.release();
      return std::unique_ptr<BinaryOpNode>(rightBinOpNode);
    }
  }
  binOpNode->Right = std::move(rightExpr);

  return std::move(binOpNode);
}
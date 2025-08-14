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
    CurrentToken = lexer.getNextToken();
    if (CurrentToken.type == tok_eof) {
      break;
    }
    if (CurrentToken.type == tok_func) {
      auto func = std::make_unique<FunctionNode>();
      func->signature = parseFunctionSignature();
      func->body = parseFunctionBody();
      root->children.push_back(std::move(func));
    }
  }

  root->print();
}

static std::map<std::string, int> PrecedenceTable = {
    {"*", 2}, {"/", 2},  {"+", 1},  {"-", 1},   {"!=", 0}, {">", 0},
    {"<", 0}, {">=", 0}, {"<=", 0}, {"===", 0}, {"==", 0}};

static std::map<std::string, std::string> BinOpTypeTable = {
    {"*", "Int"}, {"/", "Int"}, {"+", "Int"}, {"-", "Int"}, {"==", "Any"}};

std::vector<std::unique_ptr<ArgumentNode>> Parser::parseParameterList() {
  auto params = std::vector<std::unique_ptr<ArgumentNode>>();
  while (CurrentToken.type != tok_right_paren) {
    auto param = parseArgument();
    if (param) {
      params.push_back(std::move(param));
    }
    if (CurrentToken.type == tok_comma) {
      CurrentToken = lexer.getNextToken(); // Consume ','
    }
  }
  CurrentToken = lexer.getNextToken(); // Consume ')'
  return params;
}

std::unique_ptr<ArgumentNode> Parser::parseArgument() {
  if (CurrentToken.type != tok_identifier) {
    std::cerr << "Expected parameter name" << std::endl;
    return nullptr;
  }
  std::string paramName = CurrentToken.identifierName;
  CurrentToken = lexer.getNextToken(); // Consume parameter name
  if (CurrentToken.type != tok_colon) {
    std::cerr << "Expected ':' after parameter name" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume ':'
  if (CurrentToken.type != tok_identifier) {
    std::cerr << "Expected parameter type" << std::endl;
    return nullptr;
  }
  std::string paramType = CurrentToken.identifierName;
  auto param = std::make_unique<ArgumentNode>(paramName, paramType);
  CurrentToken = lexer.getNextToken(); // Consume parameter type
  return std::move(param);
}

std::unique_ptr<FunctionSignatureNode> Parser::parseFunctionSignature() {
  auto funcSignature = std::make_unique<FunctionSignatureNode>();

  std::string funcName;
  std::string returnType;

  CurrentToken = lexer.getNextToken(); // Consume the function keyword
  if (CurrentToken.type != tok_identifier) {
    std::cerr << "Expected function name after 'func'" << std::endl;
    return nullptr;
  }
  funcName = CurrentToken.identifierName;
  CurrentToken = lexer.getNextToken(); // Consume the function name
  if (CurrentToken.type != tok_left_paren) {
    std::cerr << "Expected '(' after function name" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '('
  funcSignature->parameters = parseParameterList();

  if (CurrentToken.type == tok_arrow) {
    CurrentToken = lexer.getNextToken(); // Consume '->'
    if (CurrentToken.type != tok_identifier) {
      std::cerr << "Expected return type after '->'" << std::endl;
      return nullptr;
    }
    returnType = CurrentToken.identifierName;
    CurrentToken = lexer.getNextToken(); // Consume return type
  }

  funcSignature->name = funcName;
  funcSignature->returnType = returnType;

  return std::move(funcSignature);
}

std::unique_ptr<FunctionBodyNode> Parser::parseFunctionBody() {
  auto funcBody = std::make_unique<FunctionBodyNode>();

  if (CurrentToken.type != tok_left_brace) {
    std::cerr << "Expected '{' to start function body but found "
              << CurrentToken.type << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '{'

  while (CurrentToken.type != tok_right_brace) {
    if (CurrentToken.type == tok_eof) {
      std::cerr << "Unexpected end of file in function body" << std::endl;
      return nullptr;
    }
    if (CurrentToken.type == tok_return) {
      auto returnExpr = parseReturnExpression();
      funcBody->expressions.push_back(std::move(returnExpr));
    } else {
      std::cerr << "Expected expression in function body but found:"
                << CurrentToken.type << " " << CurrentToken.identifierName
                << std::endl;

      return nullptr;
    }
  }

  if (CurrentToken.type != tok_right_brace) {
    std::cerr << "Expected '}' to end function body" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '}'
  return std::move(funcBody);
}

std::unique_ptr<ReturnNode> Parser::parseReturnExpression() {
  CurrentToken = lexer.getNextToken(); // Consume 'return'
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
  while (CurrentToken.type == tok_left_paren) {
    CurrentToken = lexer.getNextToken(); // Consume left parenthesis
    parenthesisCount++;
  }

  if (CurrentToken.type == tok_identifier) {
    expr = std::make_unique<VariableNode>(CurrentToken.identifierName);
  } else if (CurrentToken.type == tok_int) {
    expr = std::make_unique<IntNode>(CurrentToken.intValue);
  } else if (CurrentToken.type == tok_minus) {
    // parse unary minus operator
    CurrentToken = lexer.getNextToken(); // Consume minus sign
    if (CurrentToken.type != tok_int || CurrentToken.hasLeadingSpace) {
      std::cerr << "- unary operator cannot be separated from its operand"
                << std::endl;
      return nullptr;
    }
    expr = std::make_unique<IntNode>(-CurrentToken.intValue);
  } else {
    std::cerr << "Failed to parse expression: found " << CurrentToken.type
              << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume expression or Left operand
                                       // parse Binary Operator
  while (CurrentToken.type == tok_right_paren) {
    CurrentToken = lexer.getNextToken(); // Consume right parenthesis
    parenthesisCount--;
  }
  if (CurrentToken.type == tok_plus || CurrentToken.type == tok_minus ||
      CurrentToken.type == tok_multiply || CurrentToken.type == tok_divide) {
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
Parser::parseRightBinaryOpExpression(int ParenthesisCount) {
  std::string Type = BinOpTypeTable[CurrentToken.identifierName];
  auto BinOpNode = std::make_unique<BinaryOpNode>(CurrentToken.identifierName,
                                                  Type, ParenthesisCount);
  CurrentToken = lexer.getNextToken(); // Consume the operator
  auto rightExpr = parseExpression(ParenthesisCount);
  if (auto rightBinOpNode = dynamic_cast<BinaryOpNode *>(rightExpr.get())) {
    bool leftOpHasMoreParenthese =
        BinOpNode->ParenthesisCount > rightBinOpNode->ParenthesisCount;
    bool leftAndRightOpHaveSameParenthese =
        BinOpNode->ParenthesisCount == rightBinOpNode->ParenthesisCount;
    bool leftOpHasHigherOrEqaulPrecendence =
        PrecedenceTable[BinOpNode->OP] >= PrecedenceTable[rightBinOpNode->OP];
    bool leftOpIsPrioritized =
        leftOpHasMoreParenthese ||
        (leftAndRightOpHaveSameParenthese && leftOpHasHigherOrEqaulPrecendence);
    // current op is more prioritized than right op
    if (leftOpIsPrioritized) {
      BinOpNode->Right = std::move(rightBinOpNode->Left);
      rightBinOpNode->Left = std::move(BinOpNode);
      auto unused = rightExpr.release();
      return std::unique_ptr<BinaryOpNode>(rightBinOpNode);
    }
  }
  BinOpNode->Right = std::move(rightExpr);

  return std::move(BinOpNode);
}
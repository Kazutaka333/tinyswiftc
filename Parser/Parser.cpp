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
    if (CurrentToken.Type == TokEof) {
      break;
    }
    if (CurrentToken.Type == TokFunc) {
      auto Func = std::make_unique<FunctionNode>();
      Func->signature = parseFunctionSignature();
      Func->body = parseFunctionBody();
      root->Children.push_back(std::move(Func));
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
  auto Params = std::vector<std::unique_ptr<ArgumentNode>>();
  while (CurrentToken.Type != TokRightParen) {
    auto Param = parseArgument();
    if (Param) {
      Params.push_back(std::move(Param));
    }
    if (CurrentToken.Type == TokComma) {
      CurrentToken = lexer.getNextToken(); // Consume ','
    }
  }
  CurrentToken = lexer.getNextToken(); // Consume ')'
  return Params;
}

std::unique_ptr<ArgumentNode> Parser::parseArgument() {
  if (CurrentToken.Type != TokIdentifier) {
    std::cerr << "Expected parameter name" << std::endl;
    return nullptr;
  }
  std::string ParamName = CurrentToken.IdentifierName;
  CurrentToken = lexer.getNextToken(); // Consume parameter name
  if (CurrentToken.Type != TokColon) {
    std::cerr << "Expected ':' after parameter name" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume ':'
  if (CurrentToken.Type != TokIdentifier) {
    std::cerr << "Expected parameter type" << std::endl;
    return nullptr;
  }
  std::string ParamType = CurrentToken.IdentifierName;
  auto Param = std::make_unique<ArgumentNode>(ParamName, ParamType);
  CurrentToken = lexer.getNextToken(); // Consume parameter type
  return Param;
}

std::unique_ptr<FunctionSignatureNode> Parser::parseFunctionSignature() {
  auto FuncSignature = std::make_unique<FunctionSignatureNode>();

  std::string FuncName;
  std::string ReturnType;

  CurrentToken = lexer.getNextToken(); // Consume the function keyword
  if (CurrentToken.Type != TokIdentifier) {
    std::cerr << "Expected function name after 'func'" << std::endl;
    return nullptr;
  }
  FuncName = CurrentToken.IdentifierName;
  CurrentToken = lexer.getNextToken(); // Consume the function name
  if (CurrentToken.Type != TokLeftParen) {
    std::cerr << "Expected '(' after function name" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '('
  FuncSignature->parameters = parseParameterList();

  if (CurrentToken.Type == TokArrow) {
    CurrentToken = lexer.getNextToken(); // Consume '->'
    if (CurrentToken.Type != TokIdentifier) {
      std::cerr << "Expected return type after '->'" << std::endl;
      return nullptr;
    }
    ReturnType = CurrentToken.IdentifierName;
    CurrentToken = lexer.getNextToken(); // Consume return type
  }

  FuncSignature->name = FuncName;
  FuncSignature->returnType = ReturnType;

  return FuncSignature;
}

std::unique_ptr<FunctionBodyNode> Parser::parseFunctionBody() {
  auto FuncBody = std::make_unique<FunctionBodyNode>();

  if (CurrentToken.Type != TokLeftBrace) {
    std::cerr << "Expected '{' to start function body but found "
              << CurrentToken.Type << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '{'

  while (CurrentToken.Type != TokRightBrace) {
    if (CurrentToken.Type == TokEof) {
      std::cerr << "Unexpected end of file in function body" << std::endl;
      return nullptr;
    }
    if (CurrentToken.Type == TokReturn) {
      auto ReturnExpr = parseReturnExpression();
      FuncBody->expressions.push_back(std::move(ReturnExpr));
    } else {
      std::cerr << "Expected expression in function body but found:"
                << CurrentToken.Type << " " << CurrentToken.IdentifierName
                << std::endl;

      return nullptr;
    }
  }

  if (CurrentToken.Type != TokRightBrace) {
    std::cerr << "Expected '}' to end function body" << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume '}'
  return FuncBody;
}

std::unique_ptr<ReturnNode> Parser::parseReturnExpression() {
  CurrentToken = lexer.getNextToken(); // Consume 'return'
  auto ReturnExpr = std::make_unique<ReturnNode>();

  std::unique_ptr<ExprNode> Expr = parseExpression(0);
  if (!Expr) {
    std::cerr << "Expected expression after 'return'" << std::endl;
    return nullptr;
  }

  ReturnExpr->expression = std::move(Expr);
  return ReturnExpr;
}

std::unique_ptr<ExprNode> Parser::parseExpression(int parenthesisCount) {
  std::unique_ptr<ExprNode> Expr;
  while (CurrentToken.Type == TokLeftParen) {
    CurrentToken = lexer.getNextToken(); // Consume left parenthesis
    parenthesisCount++;
  }

  if (CurrentToken.Type == TokIdentifier) {
    Expr = std::make_unique<VariableNode>(CurrentToken.IdentifierName);
  } else if (CurrentToken.Type == TokInt) {
    Expr = std::make_unique<IntNode>(CurrentToken.IntValue);
  } else if (CurrentToken.Type == TokMinus) {
    // parse unary minus operator
    CurrentToken = lexer.getNextToken(); // Consume minus sign
    if (CurrentToken.Type != TokInt || CurrentToken.HasLeadingSpace) {
      std::cerr << "- unary operator cannot be separated from its operand"
                << std::endl;
      return nullptr;
    }
    Expr = std::make_unique<IntNode>(-CurrentToken.IntValue);
  } else {
    std::cerr << "Failed to parse expression: found " << CurrentToken.Type
              << std::endl;
    return nullptr;
  }
  CurrentToken = lexer.getNextToken(); // Consume expression or Left operand
                                       // parse Binary Operator
  while (CurrentToken.Type == TokRightParen) {
    CurrentToken = lexer.getNextToken(); // Consume right parenthesis
    parenthesisCount--;
  }
  if (CurrentToken.Type == TokPlus || CurrentToken.Type == TokMinus ||
      CurrentToken.Type == TokMultiply || CurrentToken.Type == TokDivide) {
    auto BinOpNode = parseRightBinaryOpExpression(parenthesisCount);
    BinaryOpNode *TempNode = BinOpNode.get();
    while (auto TempBinNode =
               dynamic_cast<BinaryOpNode *>(TempNode->Left.get())) {
      TempNode = TempBinNode;
    }
    TempNode->Left = std::move(Expr);
    return std::move(BinOpNode);
  }

  return Expr;
}

std::unique_ptr<BinaryOpNode>
Parser::parseRightBinaryOpExpression(int ParenthesisCount) {
  std::string Type = BinOpTypeTable[CurrentToken.IdentifierName];
  auto BinOpNode = std::make_unique<BinaryOpNode>(CurrentToken.IdentifierName,
                                                  Type, ParenthesisCount);
  CurrentToken = lexer.getNextToken(); // Consume the operator
  auto RightExpr = parseExpression(ParenthesisCount);
  if (auto RightBinOpNode = dynamic_cast<BinaryOpNode *>(RightExpr.get())) {
    bool LeftOpHasMoreParenthese =
        BinOpNode->ParenthesisCount > RightBinOpNode->ParenthesisCount;
    bool LeftAndRightOpHaveSameParenthese =
        BinOpNode->ParenthesisCount == RightBinOpNode->ParenthesisCount;
    bool LeftOpHasHigherOrEqaulPrecendence =
        PrecedenceTable[BinOpNode->OP] >= PrecedenceTable[RightBinOpNode->OP];
    bool LeftOpIsPrioritized =
        LeftOpHasMoreParenthese ||
        (LeftAndRightOpHaveSameParenthese && LeftOpHasHigherOrEqaulPrecendence);
    // current op is more prioritized than right op
    if (LeftOpIsPrioritized) {
      BinOpNode->Right = std::move(RightBinOpNode->Left);
      RightBinOpNode->Left = std::move(BinOpNode);
      RightExpr.release();
      return std::unique_ptr<BinaryOpNode>(RightBinOpNode);
    }
  }
  BinOpNode->Right = std::move(RightExpr);

  return BinOpNode;
}
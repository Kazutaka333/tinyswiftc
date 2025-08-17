// Lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>

enum TokenType {
  TokIdentifier,
  TokFunc,
  TokReturn,
  TokEof,
  TokLeftParen,
  TokRightParen,
  TokColon,
  TokComma,
  TokArrow,
  TokLeftBrace,
  TokRightBrace,
  TokInt,
  TokPlus,
  TokMinus,
  TokMultiply,
  TokDivide,
  TokEqual
};

std::ostream &operator<<(std::ostream &os, TokenType type);

struct Token {
  TokenType Type;
  std::string IdentifierName;
  int IntValue = 0;
  bool HasLeadingSpace = false;

  Token() = default;
  Token(const TokenType Type, const std::string &IdentifierName = "")
      : Type(Type), IdentifierName(IdentifierName) {};
  Token(const TokenType Type, const int IntValue, const bool HasLeadingSpace)
      : Type(Type), IntValue(IntValue), HasLeadingSpace(HasLeadingSpace) {};
};

class Lexer {
  std::string Filename;
  std::ifstream File;

public:
  Lexer(const std::string &Filename);
  Token getNextToken();
  Token peekNextToken();
};
#endif
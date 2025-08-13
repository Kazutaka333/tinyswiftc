// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>

enum TokenType {
  tok_identifier,
  tok_func,
  tok_return,
  tok_eof,
  tok_left_paren,
  tok_right_paren,
  tok_colon,
  tok_comma,
  tok_arrow,
  tok_left_brace,
  tok_right_brace,
  tok_int,
  tok_plus,
  tok_minus,
  tok_multiply,
  tok_divide
};

std::ostream &operator<<(std::ostream &os, TokenType type);

struct Token {
  TokenType type;
  std::string identifierName;
  int intValue;
  bool hasLeadingSpace;

  Token() = default;
  Token(const TokenType type, const std::string &identifierName = "")
      : type(type), identifierName(identifierName) {};
  Token(const TokenType type, const int intValue, const bool hasLeadingSpace)
      : type(type), intValue(intValue), hasLeadingSpace(hasLeadingSpace) {};
};

class Lexer {
  std::string filename;
  std::ifstream file;

public:
  Lexer(const std::string &filename);
  Token getNextToken();
  Token peekNextToken();
};
#endif
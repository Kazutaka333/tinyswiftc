#include "Lexer.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream> // Required for std::ostream
#include <string>
#include <unordered_set>

Lexer::Lexer(const std::string &filename) : Filename(filename) {
  File.open(filename);
  if (!File.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }
}

bool skipSpace(std::ifstream &file) {
  char Ch;
  bool HasSkipped = false;
  while (isspace(file.peek())) {
    file.get(Ch);
    HasSkipped = true;
  }
  return HasSkipped;
}

Token Lexer::getNextToken() {
  char Ch;
  std::unordered_set<char> Delimiters = {',', '.', '(', ')', '{',
                                         '}', ':', '-', EOF};
  std::string Buffer;
  bool HasLeadingSpace;
  HasLeadingSpace = skipSpace(File);

  // scan for token
  while (File.get(Ch)) {
    Buffer += Ch;
    char NextChar = File.peek();

    if (Buffer == "//") {
      while (Ch != '\n') {
        File.get(Ch);
      }
      Buffer.clear();
      HasLeadingSpace = skipSpace(File);
      continue;
    }

    if (Ch == '(')
      return Token(TokLeftParen, Buffer);
    if (Ch == ')')
      return Token(TokRightParen, Buffer);
    if (Ch == ':')
      return Token(TokColon, Buffer);
    if (Ch == ',')
      return Token(TokComma, Buffer);
    if (Ch == '{')
      return Token(TokLeftBrace, Buffer);
    if (Ch == '}')
      return Token(TokRightBrace, Buffer);
    if (Ch == '+')
      return Token(TokPlus, Buffer);
    if (Ch == '-' && NextChar != '>')
      return Token(TokMinus, Buffer);
    if (Ch == '*')
      return Token(TokMultiply, Buffer);
    if (Ch == '/' && NextChar != '/')
      return Token(TokDivide, Buffer);
    if (Buffer == "->")
      return Token(TokArrow, Buffer);
    if (Buffer == "==")
      return Token(TokEqual, Buffer);

    // tokenizing keyword or identifier
    if (isspace(NextChar) || Delimiters.find(NextChar) != Delimiters.end()) {
      if (Buffer == "func")
        return Token(TokFunc, Buffer);
      if (Buffer == "return")
        return Token(TokReturn, Buffer);
      if (std::all_of(Buffer.begin(), Buffer.end(), ::isdigit)) {
        int Value = strtod(Buffer.c_str(), 0);
        return Token(TokInt, Value, HasLeadingSpace);
      }
      return Token(TokIdentifier, Buffer);
    }
  }
  return Token(TokEof);
}

Token Lexer::peekNextToken() {
  std::streampos CurrentPos = File.tellg();
  Token NextToken = getNextToken();
  File.seekg(CurrentPos);
  return NextToken;
}

std::ostream &operator<<(std::ostream &os, TokenType type) {
  switch (type) {
  case TokIdentifier:
    os << "Identifier";
    break;
  case TokFunc:
    os << "Function";
    break;
  case TokReturn:
    os << "Return";
    break;
  case TokEof:
    os << "End of File";
    break;
  case TokLeftParen:
    os << "Left Paren";
    break;
  case TokRightParen:
    os << "Right Paren";
    break;
  case TokColon:
    os << "Colon";
    break;
  case TokComma:
    os << "Comma";
    break;
  case TokArrow:
    os << "Arrow";
    break;
  case TokLeftBrace:
    os << "Left Brace";
    break;
  case TokRightBrace:
    os << "Right Brace";
    break;
  default:
    os << "Unknown";
    break;
  }
  return os;
}

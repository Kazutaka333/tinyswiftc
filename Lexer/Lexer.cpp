#include "Lexer.h"
#include "../Util/debug_log.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream> // Required for std::ostream
#include <string>
#include <unordered_set>

Lexer::Lexer(const std::string &filename) : filename(filename) {
  file.open(filename);
  if (!file.is_open()) {
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
  HasLeadingSpace = skipSpace(file);

  // scan for token
  while (file.get(Ch)) {
    Buffer += Ch;
    char NextChar = file.peek();

    if (Buffer == "//") {
      while (Ch != '\n') {
        file.get(Ch);
      }
      Buffer.clear();
      HasLeadingSpace = skipSpace(file);
      continue;
    }

    if (Ch == '(')
      return Token(tok_left_paren, Buffer);
    if (Ch == ')')
      return Token(tok_right_paren, Buffer);
    if (Ch == ':')
      return Token(tok_colon, Buffer);
    if (Ch == ',')
      return Token(tok_comma, Buffer);
    if (Ch == '{')
      return Token(tok_left_brace, Buffer);
    if (Ch == '}')
      return Token(tok_right_brace, Buffer);
    if (Ch == '+')
      return Token(tok_plus, Buffer);
    if (Ch == '-' && NextChar != '>')
      return Token(tok_minus, Buffer);
    if (Ch == '*')
      return Token(tok_multiply, Buffer);
    if (Ch == '/' && NextChar != '/')
      return Token(tok_divide, Buffer);
    if (Buffer == "->")
      return Token(tok_arrow, Buffer);
    if (Buffer == "==")
      return Token(tok_equal, Buffer);

    // tokenizing keyword or identifier
    if (isspace(NextChar) || Delimiters.find(NextChar) != Delimiters.end()) {
      if (Buffer == "func")
        return Token(tok_func, Buffer);
      if (Buffer == "return")
        return Token(tok_return, Buffer);
      if (std::all_of(Buffer.begin(), Buffer.end(), ::isdigit)) {
        int Value = strtod(Buffer.c_str(), 0);
        return Token(tok_int, Value, HasLeadingSpace);
      }
      return Token(tok_identifier, Buffer);
    }
  }
  return Token(tok_eof);
}

Token Lexer::peekNextToken() {
  std::streampos CurrentPos = file.tellg();
  Token NextToken = getNextToken();
  file.seekg(CurrentPos);
  return NextToken;
}

std::ostream &operator<<(std::ostream &os, TokenType type) {
  switch (type) {
  case tok_identifier:
    os << "Identifier";
    break;
  case tok_func:
    os << "Function";
    break;
  case tok_return:
    os << "Return";
    break;
  case tok_eof:
    os << "End of File";
    break;
  case tok_left_paren:
    os << "Left Paren";
    break;
  case tok_right_paren:
    os << "Right Paren";
    break;
  case tok_colon:
    os << "Colon";
    break;
  case tok_comma:
    os << "Comma";
    break;
  case tok_arrow:
    os << "Arrow";
    break;
  case tok_left_brace:
    os << "Left Brace";
    break;
  case tok_right_brace:
    os << "Right Brace";
    break;
  default:
    os << "Unknown";
    break;
  }
  return os;
}

#include "Lexer.h"
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

Token Lexer::getNextToken() {
  char ch;
  std::unordered_set<char> delimiters = {',', '.', '(', ')', '{',
                                         '}', ':', '-', EOF};
  std::string buffer;
  buffer.clear();
  // skip leading spacing characters
  while (isspace(file.peek())) {
    file.get(ch);
  }
  // scan for token
  while (file.get(ch)) {
    buffer += ch;
    if (ch == '(')
      return Token(tok_left_paren, buffer);
    if (ch == ')')
      return Token(tok_right_paren, buffer);
    if (ch == ':')
      return Token(tok_colon, buffer);
    if (ch == ',')
      return Token(tok_comma, buffer);
    if (ch == '{')
      return Token(tok_left_brace, buffer);
    if (ch == '}')
      return Token(tok_right_brace, buffer);
    if (buffer == "->")
      return Token(tok_arrow, buffer);
    char nextChar = file.peek();
    if (isspace(nextChar) || delimiters.find(nextChar) != delimiters.end()) {
      if (buffer == "func")
        return Token(tok_func, buffer);
      if (buffer == "return")
        return Token(tok_return, buffer);
      if (std::all_of(buffer.begin(), buffer.end(), ::isdigit)) {
        int value = strtod(buffer.c_str(), 0);
        return Token(tok_int, value);
      }
      return Token(tok_identifier, buffer);
    }
  }
  return Token(tok_eof);
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

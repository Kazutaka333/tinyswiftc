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
  char ch;
  bool hasSkipped = false;
  while (isspace(file.peek())) {
    file.get(ch);
    hasSkipped = true;
  }
  return hasSkipped;
}

Token Lexer::getNextToken() {
  char ch;
  std::unordered_set<char> delimiters = {',', '.', '(', ')', '{',
                                         '}', ':', '-', EOF};
  std::string buffer;
  bool hasLeadingSpace;
  hasLeadingSpace = skipSpace(file);

  // scan for token
  while (file.get(ch)) {
    buffer += ch;
    char nextChar = file.peek();

    if (buffer == "//") {
      while (ch != '\n') {
        file.get(ch);
      }
      buffer.clear();
      hasLeadingSpace = skipSpace(file);
      continue;
    }

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
    if (ch == '+')
      return Token(tok_plus, buffer);
    if (ch == '-' && nextChar != '>')
      return Token(tok_minus, buffer);
    if (ch == '*')
      return Token(tok_multiply, buffer);
    if (ch == '/' && nextChar != '/')
      return Token(tok_divide, buffer);
    if (buffer == "->")
      return Token(tok_arrow, buffer);

    // tokenizing keyword or identifier
    if (isspace(nextChar) || delimiters.find(nextChar) != delimiters.end()) {
      if (buffer == "func")
        return Token(tok_func, buffer);
      if (buffer == "return")
        return Token(tok_return, buffer);
      if (std::all_of(buffer.begin(), buffer.end(), ::isdigit)) {
        int value = strtod(buffer.c_str(), 0);
        return Token(tok_int, value, hasLeadingSpace);
      }
      return Token(tok_identifier, buffer);
    }
  }
  return Token(tok_eof);
}

Token Lexer::peekNextToken() {
  std::streampos currentPos = file.tellg();
  Token nextToken = getNextToken();
  file.seekg(currentPos);
  return nextToken;
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

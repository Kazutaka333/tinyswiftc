#include "lexer.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

enum Token {
  // Define your token types here
  tok_identifier,

};

class Lexer {
  std::string filename;
  std::ifstream file;

public:
  Lexer(const std::string &filename) : filename(filename) {
    file.open(filename);
    if (!file.is_open()) {
      std::cerr << "Error opening file: " << filename << std::endl;
      return;
    }
  }

  Token getNextToken() {
    char ch;
    std::string identifier;
    while (file.get(ch)) {
      if (!isalnum(ch))
        break;
      identifier += ch;
    }
    if (!isalnum(ch)) {
      switch (ch) {
      case 'let':
        /* code */
        break;

      default:
        break;
      }
    }
  }

  void readSwiftFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::in);
    char ch;
    if (!file) {
      std::cerr << "Error opening file: " << filename << std::endl;
      return;
    }
    std::cout << "Reading file: " << filename << std::endl;
    while (file.get(ch)) {
      if (ch == '\n') {
        std::cout << std::endl;
      } else {
        std::cout << ch;
      }
    }
    file.close();
  }
};

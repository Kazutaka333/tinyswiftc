#pragma once
#include <iostream>
#include <sstream>

extern bool debugEnabled;

template <typename... Args> inline void debug_log(Args &&...args) {
  if (!debugEnabled)
    return;
  std::ostringstream oss;
  (oss << ... << args);
  std::cout << "[DEBUG] " << oss.str() << std::endl;
}
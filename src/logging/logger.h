#pragma once

#include <iostream>
#include <source_location>

#ifndef LEVEL
#define LEVEL 0
#endif

namespace logger {

enum class Level {
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
};

template <class... Args>
inline void info(const Args&... joining) {
  log(Level::INFO, joining...);
}

template <class... Args>
inline void warning(const Args&... joining) {
  log(Level::WARNING, joining...);
}

template <class... Args>
inline void error(const Args&... joining) {
  log(Level::ERROR, joining...);
}

template <class... Args>
inline void log(Level level, const Args&... joining) {
  if (LEVEL < static_cast<int>(level)) {
    return;
  }
  switch (level) {
    case Level::INFO: {
      std::cout << "[ INFO ]";
      break;
    }
    case Level::WARNING: {
      std::cout << "[ WARNING ]";
      break;
    }
    case Level::ERROR: {
      std::cout << "[ ERROR ]";
      break;
    }
  }
  ((std::cout << " " << joining), ...) << std::endl;
}

inline std::string trim(std::string&& str) {
  str.erase(str.find_last_not_of(' ') + 1);
  str.erase(0, str.find_first_not_of(' '));
  return str;
}

inline std::string tolower(std::string&& str) {
  std::transform(str.begin(), str.end(), str.begin(), [](char ch) {
    return std::tolower(ch);
  });
  return str;
}
}  // namespace logger
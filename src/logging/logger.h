#pragma once

#include <iostream>
#include <source_location>

namespace logger {
enum class Level {
  INFO,
  WARNING,
  ERROR,
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
}  // namespace logger
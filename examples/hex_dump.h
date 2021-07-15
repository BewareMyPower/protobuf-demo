#pragma once

#include <stdio.h>

#include <sstream>

inline auto dumpBytes(const char* data, size_t size,
                      size_t count_per_line = 8) {
  std::ostringstream os;
  for (size_t i = 0; i < size; i++) {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02x", static_cast<unsigned char>(data[i]));
    if (i % count_per_line != 0) {
      os << " ";
    }
    os << buf;
    if ((i + 1) % count_per_line == 0) {
      os << "\n";
    }
  }
  return os.str();
}

#pragma once

#include <stdio.h>

#include <sstream>
#include <vector>

inline auto dumpBytes(const char* data, size_t size,
                      size_t count_per_line = 8) {
  std::ostringstream os;
  std::vector<int> line;

  for (size_t i = 0; i < size; i++) {
    char buf[3];
    snprintf(buf, sizeof(buf), "%02x", static_cast<unsigned char>(data[i]));
    line.emplace_back(static_cast<int>(data[i]));

    if (i % count_per_line != 0) {
      os << " ";
    }
    os << buf;
    if ((i + 1) % count_per_line == 0) {
      os << " |";
      for (int x : line) {
        os << " " << x;
      }
      os << "\n";
      line.clear();
    }
  }

  if (!line.empty()) {
    os << std::string((count_per_line - line.size()) * 3 + 1, ' ') << "|";
    for (int x : line) {
      os << " " << x;
    }
    os << "\n";
  }
  return os.str();
}

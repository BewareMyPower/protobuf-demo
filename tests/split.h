#pragma once

#include <string>
#include <vector>

inline auto split(const std::string& s, const std::string& separator = ",") {
  std::vector<std::string> tokens;
  size_t pos_start = 0;
  while (pos_start < s.size()) {
    const size_t pos_end = s.find_first_of(separator, pos_start);
    if (pos_end != std::string::npos) {
      if (pos_start < pos_end) {
        tokens.emplace_back(s.substr(pos_start, pos_end - pos_start));
      }
      pos_start = pos_end + 1;
    } else {
      if (pos_start < s.size()) {
        tokens.emplace_back(s.substr(pos_start));
      }
      break;
    }
  }
  return tokens;
}

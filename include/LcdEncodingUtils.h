#pragma once

#include <cstdint>
#include <vector>

namespace LcdEncoding {

inline void append16(std::vector<uint8_t> &buf, uint16_t val) {
  buf.push_back(static_cast<uint8_t>(val >> 8));
  buf.push_back(static_cast<uint8_t>(val & 0xFF));
}

inline void append32(std::vector<uint8_t> &buf, uint32_t val) {
  buf.push_back(static_cast<uint8_t>((val >> 24) & 0xFF));
  buf.push_back(static_cast<uint8_t>((val >> 16) & 0xFF));
  buf.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
  buf.push_back(static_cast<uint8_t>(val & 0xFF));
}

} // namespace LcdEncoding

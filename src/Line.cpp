#include "Line.h"
#include "LcdEncodingUtils.h"

Line &Line::color(Color c) {
  color_ = c;

  return *this;
}
Line &Line::start(uint16_t x, uint16_t y) {
  xStart_ = x;
  yStart_ = y;

  return *this;
}
Line &Line::end(uint16_t x, uint16_t y) {
  xEnd_ = x;
  yEnd_ = y;

  return *this;
}
std::vector<uint8_t> Line::encode() const {
  std::vector<uint8_t> data;
  data.reserve(11);
  data.push_back(0x03);
  LcdEncoding::append16(data, color_);
  LcdEncoding::append16(data, xStart_);
  LcdEncoding::append16(data, yStart_);
  LcdEncoding::append16(data, xEnd_);
  LcdEncoding::append16(data, yEnd_);

  return data;
}

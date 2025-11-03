#include "Rect.h"
#include "LcdEncodingUtils.h"

Rect &Rect::mode(RectMode m) {
  mode_ = m;

  return *this;
}
Rect &Rect::color(Color c) {
  color_ = c;

  return *this;
}
Rect &Rect::start(uint16_t x, uint16_t y) {
  xStart_ = x;
  yStart_ = y;

  return *this;
}
Rect &Rect::end(uint16_t x, uint16_t y) {
  xEnd_ = x;
  yEnd_ = y;

  return *this;
}

std::vector<uint8_t> Rect::encode() const {
  std::vector<uint8_t> data;
  data.reserve(12);
  data.push_back(0x05);
  data.push_back(mode_);
  LcdEncoding::append16(data, color_);
  LcdEncoding::append16(data, xStart_);
  LcdEncoding::append16(data, yStart_);
  LcdEncoding::append16(data, xEnd_);
  LcdEncoding::append16(data, yEnd_);

  return data;
}

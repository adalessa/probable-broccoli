#include "Text.h"
#include "LcdEncodingUtils.h"

Text &Text::widthAdjust(bool wa) {
  widthAdjust_ = wa;

  return *this;
}
Text &Text::color(Color c) {
  color_ = c;

  return *this;
}
Text &Text::background(Color bc) {
  bColor_ = bc;
  bShow_ = true;

  return *this;
}
Text &Text::font(Font f) {
  font_ = f;

  return *this;
}
Text &Text::position(uint16_t x, uint16_t y) {
  x_ = x;
  y_ = y;

  return *this;
}
Text::Text(const std::string &s) : string_(s) {}

std::vector<uint8_t> Text::encode() const {
  std::vector<uint8_t> data;

  data.reserve(10 + string_.length());
  data.push_back(0x11);
  data.push_back((widthAdjust_ * 0x80) | (bShow_ * 0x40) |
                 static_cast<uint8_t>(font_));
  LcdEncoding::append16(data, color_);
  LcdEncoding::append16(data, bColor_);
  LcdEncoding::append16(data, x_);
  LcdEncoding::append16(data, y_);
  data.insert(data.end(), string_.begin(), string_.end());

  return data;
}

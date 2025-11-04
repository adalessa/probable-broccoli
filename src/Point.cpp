#include "Point.h"
#include "LcdEncodingUtils.h"

Point &Point::position(uint16_t x, uint16_t y) {
  x_ = x;
  y_ = y;

  return *this;
}

Point &Point::width(uint16_t w) {
  width_ = w;

  return *this;
}

Point &Point::height(uint16_t h) {
  height_ = h;

  return *this;
}

Point &Point::color(Color c) {
  color_ = c;

  return *this;
}
std::vector<uint8_t> Point::encode() const {
  std::vector<uint8_t> data;
  data.reserve(9);
  data.push_back(0x02);
  LcdEncoding::append16(data, color_);
  data.push_back(width_);
  data.push_back(height_);
  LcdEncoding::append16(data, x_);
  LcdEncoding::append16(data, y_);

  return data;
}

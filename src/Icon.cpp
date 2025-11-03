#include "Icon.h"
#include "IcondId.h"
#include "LcdEncodingUtils.h"

Icon &Icon::position(uint8_t x, uint8_t y) {
  x_ = x;
  y_ = y;

  return *this;
}

Icon &Icon::iconSet(IconSetId set) {
  iconSet_ = set;

  return *this;
}

Icon::Icon(IcondId id) : id_(id) {}

std::vector<uint8_t> Icon::encode() const {
  std::vector<uint8_t> data;
  data.reserve(7);
  data.push_back(0x23);
  LcdEncoding::append16(data, x_);
  LcdEncoding::append16(data, y_);
  data.push_back(0x80 | iconSet_);
  data.push_back(id_);

  return data;
};

#pragma once

#include "Color.h"
#include "IEncodeable.h"
#include <cstdint>
#include <vector>

class Point : public IEncodeable {
public:
  Point &position(uint16_t x, uint16_t y);
  Point &width(uint16_t w);
  Point &height(uint16_t h);
  Point &color(Color c);
  std::vector<uint8_t> encode() const override;
private:
  uint16_t x_{0}, y_{0}, width_{5}, height_{5};
  Color color_{Color::Black};
};

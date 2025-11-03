#pragma once

#include "Color.h"
#include "IEncodeable.h"
#include <cstdint>
#include <vector>

class Line : public IEncodeable {
public:
  Line &color(Color c);
  Line &start(uint16_t x, uint16_t y);
  Line &end(uint16_t x, uint16_t y);
  std::vector<uint8_t> encode() const override;

private:
  uint16_t xStart_{0}, xEnd_{0}, yStart_{0}, yEnd_{0};
  Color color_{Color::Black};
};

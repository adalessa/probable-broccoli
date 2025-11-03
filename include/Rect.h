#pragma once

#include "Color.h"
#include "IEncodeable.h"
#include <cstdint>
#include <vector>

enum RectMode : uint8_t {
  Frame = 0,
  Fill = 1,
  XORFill = 2,
};

class Rect : public IEncodeable {
public:
  Rect &mode(RectMode m);
  Rect &color(Color c);
  Rect &start(uint16_t x, uint16_t y);
  Rect &end(uint16_t x, uint16_t y);
  std::vector<uint8_t> encode() const override;

private:
  RectMode mode_{RectMode::Fill};
  uint16_t xStart_{0}, xEnd_{0}, yStart_{0}, yEnd_{0};
  Color color_{Color::Black};
};


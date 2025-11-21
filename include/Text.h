#pragma once

#include "Color.h"
#include "Font.h"
#include "IEncodeable.h"
#include <string>
/*
  *bool widthAdjust, bool bShow, uint8_t size, uint16_t color,
               uint16_t bColor, uint16_t x, uint16_t y,
               const std::string &string
  * */

class Text : public IEncodeable {
public:
  Text &widthAdjust(bool wa);
  Text &color(Color c);
  Text &background(Color bc);
  Text &font(Font f);
  Text &position(uint16_t x, uint16_t y);
  Text(const std::string &s, uint16_t displayWidth = 0);
  Text &displayWidth(uint16_t w);

  std::vector<uint8_t> encode() const override;
private:
  bool widthAdjust_{false}, bShow_{false};
  Color color_{Color::White}, bColor_{Color::Black};
  Font font_{Font::x12x24};
  uint16_t x_{0}, y_{0};
  std::string string_{""};
  uint16_t displayWidth_{0};
};

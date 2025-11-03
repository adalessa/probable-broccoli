#pragma once

#include "IEncodeable.h"
#include "IcondId.h"

class Icon: public IEncodeable {
public:
  Icon(IcondId id);
  Icon &position(uint8_t x, uint8_t y);
  Icon &iconSet(IconSetId set);
  std::vector<uint8_t> encode() const override;
private:
  IcondId id_;
  IconSetId iconSet_{IconSetId::Main};
  uint8_t x_{0}, y_{0};
};

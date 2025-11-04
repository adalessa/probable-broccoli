#pragma once

#include <AiEsp32RotaryEncoder.h>

class Input {
public:
  void setEncoder(AiEsp32RotaryEncoder *enc) { encoder = enc; }
  AiEsp32RotaryEncoder *getEncoder() { return encoder; }

private:
  AiEsp32RotaryEncoder *encoder;
};

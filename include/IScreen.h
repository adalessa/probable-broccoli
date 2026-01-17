#pragma once

#include "WString.h"
#include <ArduinoJson.h>
class IScreen {
public:
  virtual void init() = 0;
  virtual void update() = 0;
  virtual String getName() = 0;
  virtual void updateData(JsonObject data) = 0;
};

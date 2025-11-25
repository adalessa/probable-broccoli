#pragma once

#include "BaseComponent.h"

class HelloWorldComponent : public BaseComponent {
public:
  HelloWorldComponent(LcdApi &lcd, AppState &state);
  void update(Input input) override;

private:
  bool needsDraw = true;
  int previousCounter = 0;
};


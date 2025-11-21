#pragma once

#include "IComponent.h"
#include "LcdApi.h"

class HelloWorldComponent : public IComponent {
public:
  HelloWorldComponent(LcdApi &lcd) : lcd(lcd) {}
  void update(Input input) override;

private:
  LcdApi &lcd;
  bool needsDraw = true;
};

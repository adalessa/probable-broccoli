#pragma once

#include "IComponent.h"
#include "Input.h"
#include "LcdApi.h"
#include "Text.h"

class App {
public:
  void update(Input input) {
    if (component) {
      component->update(input);
    } else {
      lcd.draw(
        Text("No component set")
          .color(Color::Red)
          .position(10, 10)
      );
    }
  }
  void setComponent(IComponent *comp) { component = comp; }
  App(LcdApi &lcdApi) : lcd(lcdApi), component(nullptr) {}

private:
  LcdApi &lcd;
  IComponent *component;
};

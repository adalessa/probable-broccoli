#pragma once

#include "AppMode.h"
#include "AppState.h"
#include "HelloWorldComponent.h"
#include "IComponent.h"
#include "Input.h"
#include "KlipperComponent.h"
#include "LcdApi.h"
#include "Text.h"

class App {
public:
  void update(Input input) {
    // can handle here some menu if I want like a pause menu
    if (input.getEncoder()->isEncoderButtonClicked()) {
        // can activate menu, and wait there without calling the component
    }
    if (component) {
      component->update(input);
    } else {
      lcd.draw(Text("No component set").color(Color::Red).position(10, 10));
    }
  }

  void changeMode(AppMode mode) {
    switch (mode) {
    case AppMode::Klipper: {
      std::vector<PrinterConfig> printers = {{"10.27.22.60", "Silent"},
                                             {"10.27.22.61", "Sprite"}};
      setComponent(new KlipperComponent(lcd, getState(), printers));
      break;
    }
    case AppMode::Home:
      setComponent(new HelloWorldComponent(lcd, getState()));
      break;
    case AppMode::Tasks:
      setComponent(new HelloWorldComponent(lcd, getState()));
      break;
    default:
      setComponent(nullptr);
    }
    appMode = mode;
  }

  App(LcdApi &lcdApi) : lcd(lcdApi) {}

  AppState &getState() { return state; }

private:
  void setComponent(IComponent *comp) { component = comp; }
  LcdApi &lcd;
  IComponent *component;
  AppState state;
  AppMode appMode;
};

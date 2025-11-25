#pragma once
#include "IComponent.h"
#include "LcdApi.h"
#include "AppState.h"

class BaseComponent : public IComponent {
protected:
  LcdApi& lcd;
  AppState& state;
public:
  BaseComponent(LcdApi& lcd, AppState& state) : lcd(lcd), state(state) {}
};

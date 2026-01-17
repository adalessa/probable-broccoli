#pragma once

#include "LcdApi.h"
class ConnectingScreen {
protected:
  LcdApi& lcd;
public:
  ConnectingScreen(LcdApi& lcd): lcd(lcd) {};
  void init();
};

#pragma once

#include "BaseComponent.h"

class MessageComponent : public BaseComponent {
public:
  MessageComponent(LcdApi &lcd, AppState &state);
  void update(Input input) override;

private:
  int x = 10;
};

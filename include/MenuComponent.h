#pragma once

#include "BaseComponent.h"

class MenuComponent : public BaseComponent {
public:
  MenuComponent(LcdApi &lcd, AppState &state);
  void update(Input input) override;

private:
  bool needsDraw = true;
};

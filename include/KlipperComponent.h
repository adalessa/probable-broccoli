#pragma once

#include "IComponent.h"
#include "Input.h"
#include "LcdApi.h"

class KlipperComponent : public IComponent {
public:
  KlipperComponent(LcdApi &lcd);
  void update(Input input) override;

private:
  LcdApi &lcd;
  void draw();
  void fetchData();
  bool needsUpdate_{true};
  float bedTemp_{0};
  float bedTarget_{0};
  float extruderTemp_{0};
  float extruderTarget_{0};
  unsigned long lastUpdate_{0};
  float printProgress_{0};
  std::string printState_{"unknown"};
};

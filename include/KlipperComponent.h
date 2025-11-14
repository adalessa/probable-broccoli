#pragma once

#include "IComponent.h"
#include "Input.h"
#include "LcdApi.h"
#include <vector>
#include <string>

struct PrinterInfo {
  std::string ip;
  std::string name;
  float bedTemp = 0;
  float bedTarget = 0;
  float extruderTemp = 0;
  float extruderTarget = 0;
  float printProgress = 0;
  std::string printState = "unknown";
  unsigned long lastUpdate = 0;
  bool needsUpdate = true;
};

class KlipperComponent : public IComponent {
public:
  KlipperComponent(LcdApi &lcd, const std::vector<std::string> &printerIps);
  void update(Input input) override;

private:
  LcdApi &lcd;
  std::vector<PrinterInfo> printers;
  void draw();
  void fetchData();
};

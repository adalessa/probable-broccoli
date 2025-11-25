#pragma once
#include "BaseComponent.h"
#include "Input.h"
#include <vector>
#include <string>

struct PrinterConfig {
  std::string ip;
  std::string name;
};

struct PrinterInfo {
  std::string ip;
  std::string name;
  float bedTemp = 0;
  float bedTarget = 0;
  float extruderTemp = 0;
  float extruderTarget = 0;
  float printProgress = 0;
  std::string printState = "unknown";
  int currentLayer = 0;
  int totalLayer = 0;
  unsigned long lastUpdate = 0;
  bool needsUpdate = true;
};

class KlipperComponent : public BaseComponent {
public:
  KlipperComponent(LcdApi &lcd, AppState &state, const std::vector<PrinterConfig> &printerConfigs);
  void update(Input input) override;

private:
  std::vector<PrinterInfo> printers;
  void draw();
  void fetchData();
};


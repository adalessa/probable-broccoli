#pragma once

#include "IComponent.h"
#include "Input.h"
#include "LcdApi.h"

struct Task {
  std::string title;
  bool completed;
};

class TasksComponent : public IComponent {
public:
  TasksComponent(LcdApi &lcd, Input &input);
  void update(Input input) override;
private:
  void draw();
  LcdApi &lcd;
  bool needsUpdate_{true};
  std::vector<Task> tasks_{};
  int selectedTasksIndex{0};
};

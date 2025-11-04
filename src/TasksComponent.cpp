#include "TasksComponent.h"
#include "Rect.h"
#include "Text.h"
#include <algorithm>

TasksComponent::TasksComponent(LcdApi &lcd, Input &input) : lcd(lcd) {
  tasks_.push_back({"Some task", false});
  tasks_.push_back({"Another task", true});
  tasks_.push_back({"Tasks ui", false});

  input.getEncoder()->setBoundaries(0, tasks_.size() - 1, true);
}

void TasksComponent::update(Input input) {

  if (input.getEncoder()->encoderChanged()) {
    selectedTasksIndex = input.getEncoder()->readEncoder();
    needsUpdate_ = true;
  }
  if (input.getEncoder()->isEncoderButtonClicked()) {
    tasks_[selectedTasksIndex].completed =
        !tasks_[selectedTasksIndex].completed;
    needsUpdate_ = true;
  }

  if (needsUpdate_) {
    draw();
    needsUpdate_ = false;
  }
}

void TasksComponent::draw() {
  lcd.draw(Rect().color(Color::DarkGreen).start(0, 0).end(LCD_WIDTH, 24));
  lcd.draw(Text("Tasks").color(Color::Black).position(5, 0));

  // base on the tasks build an string [<completed>/<total>]
  int completed = std::count_if(tasks_.begin(), tasks_.end(),
                                [](const Task &t) { return t.completed; });
  int total = tasks_.size();

  std::string tasks =
      "[" + std::to_string(completed) + "/" + std::to_string(total) + "]";

  lcd.draw(Text(tasks)
               .color(Color::Black)
               .position(LCD_WIDTH - (tasks.length() * 12), 0));

  for (size_t i = 0; i < tasks_.size(); ++i) {
    const Task &task = tasks_[i];
    std::string marker = (i == selectedTasksIndex) ? "> " : "  ";
    std::string checkbox = task.completed ? "[X] " : "[ ] ";
    lcd.draw(Text(marker + checkbox + task.title)
                 .background(Color::Black)
                 .color(Color::DarkGreen)
                 .position(5, 30 + static_cast<uint16_t>(i) * 30));
  }
}

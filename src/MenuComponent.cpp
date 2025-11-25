#include "MenuComponent.h"
#include "Text.h"

MenuComponent::MenuComponent(LcdApi &lcd, AppState &state)
    : BaseComponent(lcd, state) {
  lcd.clear(Color::Blue);
}

void MenuComponent::update(Input input) {
  if (needsDraw) {
    lcd.draw(Text("Menu Component")
                 .color(Color::DarkGreen)
                 .background(Color::Black)
                 .position(10, 10));
    needsDraw = false;
  }
}

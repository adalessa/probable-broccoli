#include "MessageComponent.h"
#include "Color.h"
#include "Font.h"
#include "LcdApi.h"
#include "Text.h"

MessageComponent::MessageComponent(LcdApi &lcd, AppState &state)
    : BaseComponent(lcd, state) {
  lcd.clear(Color::Black);
}

void MessageComponent::update(Input input) {
  unsigned long now = millis();
  x += 4;
  if (x > LCD_WIDTH)
    x = 0;
  lcd.draw(Text(state.getMessage())
               .font(Font::x32x64)
               .color(Color::DarkGreen)
               .background(Color::Black)
               .position(x, (LCD_HEIGHT / 2) - 32));
}

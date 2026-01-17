#include "ConnectingScreen.h"
#include "LcdApi.h"
#include "Text.h"

void ConnectingScreen::init() {
  lcd.clear(Color::Black);

  lcd.draw(Text("Connecting...")
               .font(Font::x32x64)
               .color(Color::White)
               .position(20, LCD_HEIGHT / 2 - 32));
}

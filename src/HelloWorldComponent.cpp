#include "HelloWorldComponent.h"
#include "Text.h"

void HelloWorldComponent::update(Input input) {
    if (needsDraw) {
        lcd.draw(Text("Hello, World!")
                     .color(Color::White)
                     .background(Color::Black)
                     .position(10, 10));

        needsDraw = false;
    }
};

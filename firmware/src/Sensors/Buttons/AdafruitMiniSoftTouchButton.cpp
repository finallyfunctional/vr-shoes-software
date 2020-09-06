#include "../../../include/Sensors/Buttons/AdafruitMiniSoftTouchButton.h"

AdafruitMiniSoftTouchButton::AdafruitMiniSoftTouchButton(int buttonPin)
{
    this->buttonPin = buttonPin;
}

void AdafruitMiniSoftTouchButton::initialize()
{
    pinMode(buttonPin, INPUT_PULLUP);
}

bool AdafruitMiniSoftTouchButton::isPressed()
{
    return digitalRead(buttonPin) == LOW;
}
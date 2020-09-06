#include "../../include/Sensors/Buttons/AdafruitMiniSoftTouchButton.h"

AdafruitMiniSoftTouchButton::AdafruitMiniSoftTouchButton(int pin)
{
    this->pin = pin;
}

void AdafruitMiniSoftTouchButton::initialize()
{
    pinMode(pin, INPUT_PULLUP);
}

bool AdafruitMiniSoftTouchButton::isPressed()
{
    return digitalRead(pin) == LOW;
}
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

void AdafruitMiniSoftTouchButton::setPressedValue()
{
}

int AdafruitMiniSoftTouchButton::getPressedValue()
{
    return isPressed() ? 1 : 0;
}

int AdafruitMiniSoftTouchButton::getCurrentValue()
{
    return isPressed() ? 1 : 0;
}

void AdafruitMiniSoftTouchButton::setMaxDifferencePercentage(int diffPercentage)
{
}

int AdafruitMiniSoftTouchButton::getMaxDifferencePercentage()
{
    return 0;
}
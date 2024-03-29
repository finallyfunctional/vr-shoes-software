#include "../../../include/Sensors/Buttons/LinearHallEffectSensorButton.h"

LinearHallEffectSensorButton::LinearHallEffectSensorButton(int pin, int pressedValue)
{
    buttonPin = pin;
    this->pressedValue = pressedValue;
}

void LinearHallEffectSensorButton::initialize()
{
    
}

bool LinearHallEffectSensorButton::isPressed()
{
    int currentValue = analogRead(buttonPin);
    int currentDiffPercentage = ((float) abs(currentValue - pressedValue)) / ((float)(currentValue + pressedValue) / 2) * 100;
    return currentDiffPercentage <= percentageDiff;
}

void LinearHallEffectSensorButton::setPressedValue()
{
    this->pressedValue = analogRead(buttonPin);
}

int LinearHallEffectSensorButton::getPressedValue()
{
    return this->pressedValue;
}

int LinearHallEffectSensorButton::getCurrentValue()
{
    return analogRead(buttonPin);
}

void LinearHallEffectSensorButton::setMaxDifferencePercentage(int diffPercentage)
{
    percentageDiff = diffPercentage;
}

int LinearHallEffectSensorButton::getMaxDifferencePercentage()
{
    return this->percentageDiff;
}
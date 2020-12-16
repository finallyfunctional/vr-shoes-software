#ifndef LinearHallEffectSensorButton_h
#define LinearHallEffectSensorButton_h

#include <Arduino.h>
#include "Button.h"

class LinearHallEffectSensorButton : public Button
{
    public:
    LinearHallEffectSensorButton(int pin, int pressedValue);
    void initialize();
    bool isPressed();
    void setPressedValue();
    int getPressedValue();
    void setMaxDifferencePercentage(int diffPercentage);
    int getMaxDifferencePercentage();

    private:
    int buttonPin;
    int pressedValue;
    int percentageDiff;
};

#endif
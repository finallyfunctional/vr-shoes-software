#ifndef AdafruitMiniSoftTouchButton_h
#define AdafruitMiniSoftTouchButton_h

#include <Arduino.h>
#include "Button.h"

//https://www.adafruit.com/product/3983
class AdafruitMiniSoftTouchButton : public Button 
{
    public:
    AdafruitMiniSoftTouchButton(int buttonPin);
    void initialize();
    bool isPressed();
    void setPressedValue();
    int getPressedValue();
    int getCurrentValue();
    void setMaxDifferencePercentage(int diffPercentage);
    int getMaxDifferencePercentage();

    private:
    int buttonPin;
};

#endif
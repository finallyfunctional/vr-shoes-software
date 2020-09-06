#ifndef AdafruitMiniSoftTouchButton_h
#define AdafruitMiniSoftTouchButton_h

#include <Arduino.h>
#include "Button.h"

class AdafruitMiniSoftTouchButton : Button 
{
    public:
    AdafruitMiniSoftTouchButton(int pin);
    void initialize();
    bool isPressed();

    private:
    int pin;
};

#endif
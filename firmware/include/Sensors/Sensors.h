#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>
#include "../Sensors/Buttons/Button.h"

class Sensors
{
    public:
    Sensors(Button* frontButton, Button* rearButton);
    void updateSensors();
    bool isFrontButtonPressed();
    bool isRearButtonPressed();

    private:
    Button* frontButton;
    Button* rearButton;

};

#endif
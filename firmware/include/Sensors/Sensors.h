#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>
#include "../Sensors/Buttons/Button.h"
#include "./Sensors/MovementTracker.h"
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"

class Sensors
{
    public:
    Sensors(Button* frontButton, Button* rearButton, MovementTracker* movementTracker);
    void updateSensors();
    bool isFrontButtonPressed();
    bool isRearButtonPressed();
    MovementTracker* getMovementTracker();

    private:
    Button* frontButton;
    Button* rearButton;
    MovementTracker* movementTracker;
};

#endif
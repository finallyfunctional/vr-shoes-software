#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>
#include "../Sensors/Buttons/Button.h"
#include "./Sensors/MovementTracker.h"
#include "./Sensors/SpeedController.h"
#include "../Motors/Motor.h"
#include "../Wheels/Wheel.h"
#include "./Sensors/RemoteVrShoe.h"

class Sensors
{
    public:
    Sensors(Button* frontButton, Button* rearButton, MovementTracker* movementTracker, SpeedController* speedController,
            RemoteVrShoe* remoteVrShoe);
    void updateSensors();
    bool isFrontButtonPressed();
    bool isRearButtonPressed();
    MovementTracker* getMovementTracker();
    SpeedController* getSpeedController();
    RemoteVrShoe* getRemoteVrShoe();

    private:
    Button* frontButton;
    Button* rearButton;
    MovementTracker* movementTracker;
    SpeedController* speedController;
    RemoteVrShoe* remoteVrShoe;
};

#endif
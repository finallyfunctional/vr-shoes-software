#include "../../include/Sensors/Sensors.h"

Sensors::Sensors(Button* frontButton, Button* rearButton, MovementTracker* movementTracker, 
                 SpeedController* speedController, RemoteVrShoe* remoteVrShoe)
{
    this->frontButton = frontButton;
    this->rearButton = rearButton;
    this->movementTracker = movementTracker;
    this->speedController = speedController;
    this->remoteVrShoe = remoteVrShoe;
}

void Sensors::updateSensors()
{
    movementTracker->update();
}

bool Sensors::isFrontButtonPressed()
{
    return frontButton->isPressed();
}

bool Sensors::isRearButtonPressed()
{
    return rearButton->isPressed();
}

MovementTracker* Sensors::getMovementTracker()
{
    return movementTracker;
}

SpeedController* Sensors::getSpeedController()
{
    return speedController;
}

RemoteVrShoe* Sensors::getRemoteVrShoe()
{
    return remoteVrShoe;
}
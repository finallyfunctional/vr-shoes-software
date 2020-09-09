#include "../../include/Sensors/Sensors.h"

Sensors::Sensors(Button* frontButton, Button* rearButton, MovementTracker* movementTracker)
{
    this->frontButton = frontButton;
    this->rearButton = rearButton;
    this->movementTracker = movementTracker;
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
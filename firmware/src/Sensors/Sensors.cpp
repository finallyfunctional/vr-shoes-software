#include "../../include/Sensors/Sensors.h"

Sensors::Sensors(Button* frontButton, Button* rearButton)
{
    this->frontButton = frontButton;
    this->rearButton = rearButton;
}

void Sensors::updateSensors()
{
    
}

bool Sensors::isFrontButtonPressed()
{
    return frontButton->isPressed();
}

bool Sensors::isRearButtonPressed()
{
    return rearButton->isPressed();
}
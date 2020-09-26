#include "../../include/AutoShoeControllers/OmniPoweredWalking.h"

OmniPoweredWalking::OmniPoweredWalking(Sensors* sensors) : AutoShoeController(sensors)
{
    
}

void OmniPoweredWalking::update()
{
    if(!started)
    {
        return;
    }
    RemoteVrShoe* remoteShoe = sensors->getRemoteVrShoe();
    SpeedController* speedController = sensors->getSpeedController();
    MovementTracker* movementTracker = sensors->getMovementTracker();
    float speed = remoteShoe->forwardSpeed * -1;
    float distanceFromOrigin = movementTracker->getDistanceFromOrigin().getX();

    if(speed != 0 && 
       (!remoteShoe->frontButtonPressed && !remoteShoe->rearButtonPressed) &&
       (sensors->isFrontButtonPressed() && sensors->isRearButtonPressed()))
    {
        if(!moving)
        {
            if((speed < 0 && distanceFromOrigin > -0.15) ||
               (speed > 0 && distanceFromOrigin < 0.15))
            {
                speedController->setForwardSpeed(speed);
                moving = true;
            }
        }
        else 
        {
            speedController->setForwardSpeed(speed);
            moving = true;
        }
    }
    else
    {
        speedController->setRpm(0, 0);
        moving = false;
    }
}
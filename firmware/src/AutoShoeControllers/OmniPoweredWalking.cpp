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
    if(!sensors->isFrontButtonPressed() && !sensors->isRearButtonPressed()) //foot in air
    {
        speedController->setRpm(0, 0);
    }
    else if(remoteShoe->frontButtonPressed || remoteShoe->rearButtonPressed) //both feet on shoes
    {
        speedController->brakeForwardsBackwards();
        speedController->brakeSideway();
    }
    else //other foot in air
    {
        if(remoteShoe->forwardSpeed == 0) //hold position, brake
        {
            speedController->brakeForwardsBackwards();
        }
        else //match speed, opposite direction
        {
            speedController->setForwardSpeed(remoteShoe->forwardSpeed * -1);
        }
        if(remoteShoe->sidewaySpeed == 0)
        {
            speedController->brakeSideway();
        }
        else
        {
            speedController->setSidewaySpeed(remoteShoe->sidewaySpeed * -1);
        }
    }
    
}
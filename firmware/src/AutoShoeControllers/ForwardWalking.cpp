#include "../../include/AutoShoeControllers/ForwardWalking.h"

ForwardWalking::ForwardWalking(Sensors* sensors, int side) : AutoShoeController(sensors, side)
{
    
}

void ForwardWalking::start()
{
    AutoShoeController::start();
}

void ForwardWalking::update()
{
    if(!started)
    {
        return;
    }

    RemoteVrShoe* remoteShoe = sensors->getRemoteVrShoe();
    bool oppositeFootInAir = !remoteShoe->frontButtonPressed && !remoteShoe->rearButtonPressed;
    bool thisFootOnFloor = sensors->isFrontButtonPressed() || sensors->isRearButtonPressed();
    bool oppositeFootInsideBuffer = remoteShoe->forwardDistance <= 0;

    SpeedController* speedController = sensors->getSpeedController();
    movementState = ShoeMovementState::STOPPED;
    if(thisFootOnFloor)
    {
        sensors->getMovementTracker()->resetDistance();
        
        if(oppositeFootInAir && !oppositeFootInsideBuffer)
        {
            speedController->setForwardRpm(-100 * speedMultiplier);
            movementState = ShoeMovementState::WALKING;
        }
        else
        {
            speedController->setForwardRpm(0);
        }
        
    }
    else 
    {
        speedController->setForwardRpm(0);
    }
}
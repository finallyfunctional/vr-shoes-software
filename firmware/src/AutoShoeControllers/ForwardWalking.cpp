#include "../../include/AutoShoeControllers/ForwardWalking.h"

ForwardWalking::ForwardWalking(Sensors* sensors, int side) : AutoShoeController(sensors, side)
{
    strideTracker = new StrideTracker(sensors->getMovementTracker());
    otherShoeMovementTimer = new Timer(1000);
    thisFootWasInAir = false;
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
        if(thisFootWasInAir)
        {
            strideTracker->storeCurrentStrideLength();
            strideTracker->reset();
            thisFootWasInAir = false;
            otherShoeMovementTimer->start();
        }

        if(oppositeFootInAir)
        {
            if(!oppositeFootInsideBuffer)
            {
                speedController->setForwardRpm(-100 * speedMultiplier);
                movementState = ShoeMovementState::WALKING;
            }
            else
            {
                speedController->setForwardRpm(0);
            }
        }
        else if(!otherShoeMovementTimer->timeIsUp() && strideTracker->getPosition() != ShoePositionState::MIDDLE)
        {
            speedController->setForwardRpm(-100 * speedMultiplier);
            movementState = ShoeMovementState::WALKING;
        }
        else
        {
            speedController->setForwardRpm(0);
        }
        
    }
    else if(!thisFootWasInAir && strideTracker->getPosition() == ShoePositionState::BEHIND)
    {
        //speedController->brakeForwardsBackwards(); - test later
        thisFootWasInAir = true;
    }
    else
    {
        speedController->setForwardRpm(0);
        thisFootWasInAir = true;
    }
    
}
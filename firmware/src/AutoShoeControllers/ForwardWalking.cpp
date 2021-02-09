#include "../../include/AutoShoeControllers/ForwardWalking.h"

ForwardWalking::ForwardWalking(Sensors* sensors, int side) : AutoShoeController(sensors, side)
{
    strideTracker = new StrideTracker(sensors->getMovementTracker());
    otherShoeMovementTimer = new Timer();
    thisFootWasInAir = false;
}

void ForwardWalking::start()
{
    AutoShoeController::start();
}

void ForwardWalking::recenter()
{
    strideTracker->recenter();
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
    bool oppositeFootInsideBuffer = remoteShoe->forwardDistance == 0;

    SpeedController* speedController = sensors->getSpeedController();
    movementState = ShoeMovementState::STOPPED;
    if(thisFootOnFloor)
    {
        if(thisFootWasInAir)
        {
            //strideTracker->storeCurrentStrideLength();
            strideTracker->reset();
            thisFootWasInAir = false;
            if(strideTracker->getPosition() != ShoePositionState::MIDDLE)
            {
                otherShoeMovementTimer->setTimeout(500);
                otherShoeMovementTimer->start();
            }
        }

        if(oppositeFootInAir)
        {
            otherShoeMovementTimer->setTimeout(0);
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
        else if(!otherShoeMovementTimer->timeIsUp())
        {
            speedController->setForwardRpm(-100 * speedMultiplier);
            movementState = ShoeMovementState::WALKING;
        }
        else
        {
            speedController->setForwardRpm(0);
        }
        
    }
    // else if(!thisFootWasInAir && strideTracker->getPosition() == ShoePositionState::BEHIND)
    // {
    //     speedController->brakeForwardsBackwards(); - I will test this later
    //     thisFootWasInAir = true;
    // }
    else
    {
        speedController->setForwardRpm(0);
        thisFootWasInAir = true;
    }
    
}
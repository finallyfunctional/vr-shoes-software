#include "../../include/AutoShoeControllers/ForwardWalking.h"

ForwardWalking::ForwardWalking(Sensors* sensors, int side) : AutoShoeController(sensors, side)
{
    otherShoeMovementTimer = new Timer();
    thisFootWasInAir = false;
    turningBufferRadius = 0;
    turningBufferRange = new Range();
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

    SpeedController* speedController = sensors->getSpeedController();
    movementState = ShoeMovementState::STOPPED;
    if(oppositeFootInAir && oppositeFootWasOnGround && strideTracker->getPosition() == ShoePositionState::MIDDLE)
    {
        turningBufferActive = true;
        turningBufferRange->setRange(remoteShoe->forwardDistance - turningBufferRadius, 
                                     remoteShoe->forwardDistance + turningBufferRadius);
    }
    else if(turningBufferActive && !turningBufferRange->inRange(remoteShoe->forwardDistance))
    {
        turningBufferActive = false;
    }
    
    if(thisFootOnFloor)
    {
        if(thisFootWasInAir)
        {
            strideTracker->updateCurrentStrideLength();
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
            if(!turningBufferActive)
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
    else if(!thisFootWasInAir && strideTracker->getPosition() == ShoePositionState::BEHIND)
    {
        //speedController->brakeForwardsBackwards(); - I will test this later
        thisFootWasInAir = true;
        strideTracker->updateCurrentStrideLength();
        strideTracker->reset();
    }
    else
    {
        speedController->setForwardRpm(0);
        thisFootWasInAir = true;
    }
    oppositeFootWasOnGround = !oppositeFootInAir;
}
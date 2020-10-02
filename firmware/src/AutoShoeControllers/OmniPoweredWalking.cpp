#include "../../include/AutoShoeControllers/OmniPoweredWalking.h"

OmniPoweredWalking::OmniPoweredWalking(Sensors* sensors, int side) : AutoShoeController(sensors, side)
{
    
}

void OmniPoweredWalking::start()
{
    AutoShoeController::start();
    thisShoeState = 0;
    prevRemoteFootState = 0;
    remoteShoeLastPoweredXDirection = 0;
    remoteShoeLastPoweredYDirection = 0;
    thisShoeLastPoweredXDirection = 0;
    thisShoeLastPoweredYDirection = 0;
    compensateForForwardsArc = false;
    compensateForBackwardsArc = false;
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

    updateStates(remoteShoe, movementTracker);

    if(prevRemoteFootState == POWERED && remoteShoeState == UNPOWERED)
    {
        setUpArcingCompensation(remoteShoe);
    }

    if(remoteShoeState == UNPOWERED && thisShoeState == POWERED)
    {
        if(remoteShoe->forwardSpeed != 0 && 
           (!compensateForBackwardsArc || doneCompensatingForBackwardArcing(remoteShoe)) &&
           (!compensateForForwardsArc || doneCompensatingForForwardArching(remoteShoe)))
        {
            float speed = applyMinimumSpeed(remoteShoe->forwardSpeed * -1, MIN_FORWARD_SPEED) * speedMultiplier;
            speedController->setForwardSpeed(speed);
        }
        else
        {
            speedController->setForwardRpm(0);
        }
        speedController->setSidewayRpm(0);

    //     if(remoteShoe->sidewaySpeed != 0)
    //     {
    //         float speed = applyMinimumSpeed(remoteShoe->sidewaySpeed * -1, 0);
    //         if(thisShoeLastPoweredYDirection == STOPPED)
    //         {
    //             speedController->setSidewaySpeed(speed);
    //         }
    //         else if(thisShoeLastPoweredYDirection == LEFT)
    //         {
    //             if(side == ShoeSides::RIGHT)
    //             {
    //                 if(fabs(speed) > 0.3)
    //                 {
    //                     if(speed < 0)
    //                     {
    //                         speedController->setSidewaySpeed(-0.3);
    //                     }
    //                     else 
    //                     {
    //                         speedController->setSidewaySpeed(0.3);
    //                     }
    //                 }
    //                 else 
    //                 {
    //                     speedController->setSidewaySpeed(speed);
    //                 }
    //             }
    //             else
    //             {
    //                 speedController->setSidewaySpeed(speed);
    //             }
                
    //         }
    //         else if(thisShoeLastPoweredYDirection == RIGHT)
    //         {
    //             if(side == ShoeSides::LEFT)
    //             {
    //                 if(speed < 0)
    //                 {
    //                     speedController->setSidewaySpeed(-0.3);
    //                 }
    //                 else 
    //                 {
    //                     speedController->setSidewaySpeed(0.3);
    //                 }
    //             }
    //             else
    //             {
    //                 speedController->setSidewaySpeed(speed);
    //             }
    //         }
    //         else 
    //         {
    //             speedController->setSidewaySpeed(0);
    //         }
            
    //     }
    //     else
    //     {
    //         speedController->setSidewaySpeed(0);
    //     }
    }
    else 
    {
        speedController->setRpm(0, 0);
    }

    prevRemoteFootState = remoteShoeState;
    prevThisFootState = thisShoeState;
}

void OmniPoweredWalking::updateStates(RemoteVrShoe* remoteShoe, MovementTracker* movementTracker)
{
    remoteShoeState = remoteShoe->frontButtonPressed || remoteShoe->rearButtonPressed ?
        POWERED : UNPOWERED;
    thisShoeState = sensors->isFrontButtonPressed() && sensors->isRearButtonPressed() ?
        POWERED : UNPOWERED;

    if(remoteShoeState == POWERED)
    {
        remoteShoeLastPoweredXDirection = getCurrentXDirection(remoteShoe->forwardSpeed, remoteShoeLastPoweredXDirection);
        remoteShoeLastPoweredYDirection = getCurrentYDirection(remoteShoe->sidewaySpeed, remoteShoeLastPoweredYDirection);

        Vector2D currentSpeed = movementTracker->getSpeed();
        thisShoeLastPoweredXDirection = getCurrentXDirection(currentSpeed.getX(), thisShoeLastPoweredXDirection);
        thisShoeLastPoweredYDirection = getCurrentYDirection(currentSpeed.getY(), thisShoeLastPoweredYDirection);
    }
}

int OmniPoweredWalking::getCurrentXDirection(float currentSpeed, float lastDirection)
{
    if(currentSpeed > 0)
    {
        return FORWARD;
    }
    else if(currentSpeed < 0)
    {
        return BACKWARD;
    }
    else 
    {
        return lastDirection;
    }
}

int OmniPoweredWalking::getCurrentYDirection(float currentSpeed, float lastDirection)
{
    if(currentSpeed > 0)
    {
        return RIGHT;
    }
    else if(currentSpeed < 0)
    {
        return LEFT;
    }
    else 
    {
        return lastDirection;
    }
}

float OmniPoweredWalking::applyMinimumSpeed(float speed, float minSpeed)
{
    if(fabs(speed) < minSpeed)
    {
        if(speed < 0)
        {
            speed = minSpeed * -1;
        }
        else
        {
            speed = minSpeed;
        }          
    }
    return speed;
}

void OmniPoweredWalking::setUpArcingCompensation(RemoteVrShoe* remoteShoe)
{
        if(remoteShoeLastPoweredXDirection == FORWARD)
        {
            forwardLimitForArcCompensation = remoteShoe->forwardDistanceFromOrigin + ARC_DISTANCE_COMPENSATION;
            compensateForForwardsArc = true;
            compensateForBackwardsArc = false;
        }
        else if(remoteShoeLastPoweredXDirection == BACKWARD)
        {
            backwardLimitForArcCompensation =  remoteShoe->forwardDistanceFromOrigin - ARC_DISTANCE_COMPENSATION;
            compensateForBackwardsArc = true;
            compensateForForwardsArc = false;
        }
}

bool OmniPoweredWalking::doneCompensatingForBackwardArcing(RemoteVrShoe* remoteShoe)
{
    if(remoteShoe->forwardSpeed > 0 || remoteShoe->forwardDistanceFromOrigin < backwardLimitForArcCompensation)
    {
        compensateForBackwardsArc = false;
        return true;
    }
    return false;
}

bool OmniPoweredWalking::doneCompensatingForForwardArching(RemoteVrShoe* remoteShoe)
{
    if(remoteShoe->forwardSpeed < 0 || remoteShoe->forwardDistanceFromOrigin > forwardLimitForArcCompensation)
    {
        compensateForForwardsArc = false;
        return true;
    }
    return false;
}
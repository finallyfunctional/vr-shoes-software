#include "../../include/AutoShoeControllers/OmniPoweredWalking.h"

OmniPoweredWalking::OmniPoweredWalking(Sensors* sensors) : AutoShoeController(sensors)
{
    
}

void OmniPoweredWalking::start()
{
    AutoShoeController::start();
    thisShoeState = 0;
    prevRemoteFootState = 0;
    remoteShoeLastPoweredDirection = 0;
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

    int remoteShoeState = remoteShoe->frontButtonPressed || remoteShoe->rearButtonPressed ?
        POWERED : UNPOWERED;
    int thisShoeState = sensors->isFrontButtonPressed() && sensors->isRearButtonPressed() ?
        POWERED : UNPOWERED;

    if(remoteShoeState == POWERED)
    {
        if(remoteShoe->forwardSpeed > 0)
        {
            remoteShoeLastPoweredDirection = FORWARD;
        }
        else if(remoteShoe->forwardSpeed < 0)
        {
            remoteShoeLastPoweredDirection = BACKWARD;
        }
    }

    if(prevRemoteFootState == POWERED && remoteShoeState == UNPOWERED)
    {
        setUpArcingCompensation(remoteShoe);
    }

    if(remoteShoeState == UNPOWERED && thisShoeState == POWERED && remoteShoe->forwardSpeed != 0)
    {
        if((!compensateForBackwardsArc || doneCompensatingForBackwardArcing(remoteShoe)) &&
           (!compensateForForwardsArc || doneCompensatingForForwardArching(remoteShoe)))
           {
               speedController->setForwardSpeed(remoteShoe->forwardSpeed * -1 * speedMultiplier);
           }
           else
           {
               speedController->setRpm(0, 0);
           }
    }
    else 
    {
        speedController->setRpm(0, 0);
    }

    prevRemoteFootState = remoteShoeState;
}

void OmniPoweredWalking::setUpArcingCompensation(RemoteVrShoe* remoteShoe)
{
        if(remoteShoeLastPoweredDirection == FORWARD)
        {
            forwardLimitForArcCompensation = remoteShoe->forwardDistanceFromOrigin + ARC_DISTANCE_COMPENSATION;
            compensateForForwardsArc = true;
            compensateForBackwardsArc = false;
        }
        else if(remoteShoeLastPoweredDirection == BACKWARD)
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
#ifndef OmniPoweredWalking_h
#define OmniPoweredWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"
#include "../Sensors/RemoteVrShoe.h"
#include "../Sensors/SpeedController.h"
#include "../ShoeSides.h"

class OmniPoweredWalking : public AutoShoeController
{
    public:
    OmniPoweredWalking(Sensors* sensors, int side);
    void update();
    void recenter();
    void start();
    
    private:
    void setUpArcingCompensation(RemoteVrShoe* remoteShoe);
    bool doneCompensatingForBackwardArcing(RemoteVrShoe* remoteShoe);
    bool doneCompensatingForForwardArching(RemoteVrShoe* remoteShoe);
    float applyMinimumSpeed(float speed, float minSpeed);
    void updateStates(RemoteVrShoe* remoteShoe, MovementTracker* movementTracker);
    int getCurrentXDirection(float currentSpeed, float lastDirection);
    int getCurrentYDirection(float currentSpeed, float lastDirection);

    int thisShoeState;
    int remoteShoeState;
    int prevRemoteFootState;
    int prevThisFootState;
    const int JUST_POWERED_UP = 0;
    const int POWERED = 1;
    const int UNPOWERED = 2;

    int remoteShoeLastPoweredXDirection;
    int remoteShoeLastPoweredYDirection;
    int thisShoeLastPoweredXDirection;
    int thisShoeLastPoweredYDirection;
    const int STOPPED = 0;
    const int FORWARD = 1;
    const int BACKWARD = 2;
    const int LEFT = 3;
    const int RIGHT = 4;

    bool compensateForBackwardsArc;
    bool compensateForForwardsArc;
    float backwardLimitForArcCompensation;
    float forwardLimitForArcCompensation;
    const float ARC_DISTANCE_COMPENSATION = 0.04;

    const float MIN_FORWARD_SPEED = 0.2;
};

#endif
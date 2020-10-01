#ifndef OmniPoweredWalking_h
#define OmniPoweredWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"
#include "../Sensors/RemoteVrShoe.h"
#include "../Sensors/SpeedController.h"

class OmniPoweredWalking : public AutoShoeController
{
    public:
    OmniPoweredWalking(Sensors* sensors);
    void update();
    void start();
    
    private:
    void setUpArcingCompensation(RemoteVrShoe* remoteShoe);
    bool doneCompensatingForBackwardArcing(RemoteVrShoe* remoteShoe);
    bool doneCompensatingForForwardArching(RemoteVrShoe* remoteShoe);

    int thisShoeState;
    int prevRemoteFootState;
    const int POWERED = 1;
    const int UNPOWERED = 2;

    int remoteShoeLastPoweredDirection;
    const int FORWARD = 1;
    const int BACKWARD = -1;

    bool compensateForBackwardsArc;
    bool compensateForForwardsArc;
    float backwardLimitForArcCompensation;
    float forwardLimitForArcCompensation;
    const float ARC_DISTANCE_COMPENSATION = 0.04;

    const float MIN_SPEED = 0.2;
};

#endif
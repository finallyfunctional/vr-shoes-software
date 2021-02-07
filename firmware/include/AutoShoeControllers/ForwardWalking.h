#ifndef ForwardWalking_h
#define ForwardWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"
#include "../Sensors/RemoteVrShoe.h"
#include "../Sensors/SpeedController.h"
#include "../ShoeSides.h"
#include "../Timer.h"
#include "./StrideTracking/StrideTracker.h"

class ForwardWalking : public AutoShoeController
{
    public:
    ForwardWalking(Sensors* sensors, int side);
    void update();
    void start();

    private:
    bool thisFootWasInAir;
    StrideTracker* strideTracker;
    Timer* otherShoeMovementTimer;
};

#endif
#ifndef ForwardWalking_h
#define ForwardWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"
#include "../Sensors/RemoteVrShoe.h"
#include "../Sensors/SpeedController.h"
#include "../ShoeSides.h"
#include "../Timer.h"
#include "./StrideTracking/Range.h"


class ForwardWalking : public AutoShoeController
{
    public:
    ForwardWalking(Sensors* sensors, int side);
    void update();
    void recenter();
    void start();

    private:
    bool thisFootWasInAir;
    bool oppositeFootWasOnGround;
    bool turningBufferActive;
    float turningBufferRadius;
    Range* turningBufferRange;
    Timer* otherShoeMovementTimer;
};

#endif
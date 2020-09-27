#ifndef RemoteVrShoe_h
#define RemoteVrShoe_h

#include <Arduino.h>

class RemoteVrShoe
{
    public:
    String shoeId;
    bool frontButtonPressed;
    bool rearButtonPressed;
    float forwardSpeed;
    float sidewaySpeed;
    float forwardDistanceFromOrigin;
    float sidewayDistanceFromOrigin;
};

#endif
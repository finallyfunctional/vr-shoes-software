#ifndef ForwardWalking_h
#define ForwardWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"
#include "../Sensors/RemoteVrShoe.h"
#include "../Sensors/SpeedController.h"
#include "../ShoeSides.h"

class ForwardWalking : public AutoShoeController
{
    public:
    ForwardWalking(Sensors* sensors, int side);
    void update();
    void start();
};

#endif
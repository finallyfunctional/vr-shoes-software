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

    private:
    bool moving;
};

#endif
#ifndef OmniPoweredWalking_h
#define OmniPoweredWalking_h

#include <Arduino.h>
#include "./AutoShoeController.h"

class OmniPoweredWalking : public AutoShoeController
{
    public:
    OmniPoweredWalking(Sensors* sensors, Communicator* communicator);
    void update();
};

#endif
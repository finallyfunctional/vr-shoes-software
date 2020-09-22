#ifndef AutoShoeController_h
#define AutoShoeController_h

#include <Arduino.h>
#include "../Sensors/Sensors.h"
#include "../Communicators/Communicator.h"

class AutoShoeController
{
    public:
    AutoShoeController(Sensors* sensors, Communicator* communicator);
    virtual void update() = 0;

    protected:
    Sensors* sensors;
    Communicator* communicator;
};

#endif
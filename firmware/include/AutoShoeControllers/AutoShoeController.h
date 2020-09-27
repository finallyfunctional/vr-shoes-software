#ifndef AutoShoeController_h
#define AutoShoeController_h

#include <Arduino.h>
#include "../Sensors/Sensors.h"

class AutoShoeController
{
    public:
    AutoShoeController(Sensors* sensors);
    virtual void start();
    virtual void stop();
    virtual void update() = 0;

    protected:
    bool started;
    Sensors* sensors;
};

#endif
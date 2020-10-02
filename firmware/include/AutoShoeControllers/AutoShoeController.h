#ifndef AutoShoeController_h
#define AutoShoeController_h

#include <Arduino.h>
#include "../Sensors/Sensors.h"
#include "../ShoeSides.h"

class AutoShoeController
{
    public:
    AutoShoeController(Sensors* sensors, int side);
    virtual void start();
    virtual void stop();
    virtual void update() = 0;
    virtual void setSpeedMultiplier(float multiplier);
    virtual float getSpeedMultiplier();

    protected:
    bool started;
    Sensors* sensors;
    int side;
    float speedMultiplier;
};

#endif
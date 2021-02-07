#ifndef AutoShoeController_h
#define AutoShoeController_h

#include <Arduino.h>
#include <Preferences.h>
#include "../Sensors/Sensors.h"
#include "../ShoeSides.h"
#include "./States/ShoeMovementState.h"
#include "../VrShoeGlobals.h"

class AutoShoeController
{
    public:
    AutoShoeController(Sensors* sensors, int side);
    virtual void start();
    virtual void stop();
    virtual void update() = 0;
    virtual void setSpeedMultiplier(float multiplier);
    virtual float getSpeedMultiplier();
    int getMovementState();

    static const char* SPEED_MULTIPLIER_KEY;

    protected:
    bool started;
    Sensors* sensors;
    int side;
    int movementState;
    float speedMultiplier;
};

#endif
#ifndef SpeedSensor_h
#define SpeedSensor_h

#include <Arduino.h>
#include "../Vector2D.h"

class SpeedSensor
{
    public:
    virtual void updateSpeed() = 0;
    virtual Vector2D getSpeed() = 0;
};

#endif
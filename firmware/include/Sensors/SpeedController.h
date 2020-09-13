#ifndef SpeedController_h
#define SpeedController_h

#include <Arduino.h>

class SpeedController
{
    public:
    virtual void setRpm(float forwardRpm, float sidewayRpm) = 0;
};

#endif
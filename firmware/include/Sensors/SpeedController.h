#ifndef SpeedController_h
#define SpeedController_h

#include <Arduino.h>
#include "../Vector2D.h"

class SpeedController
{
    public:
    virtual void setRpm(float forwardRpm, float sidewayRpm) = 0;
    virtual void setForwardRpm(float rpm) = 0;
    virtual void setSidewayRpm(float rpm) = 0;
    virtual void setForwardSpeed(float speed) = 0;
    virtual void setSidewaySpeed(float speed) = 0;
    virtual void inverseForwardDirection() = 0;
    virtual void inverseSidewayDirection() = 0;
    virtual void resetDirections() = 0;
    virtual void brakeForwardsBackwards() = 0;
    virtual void brakeSideway() = 0;
    virtual void setDutyCycleBoost(float boost) = 0;
    virtual Vector2D getCurrentDutyCycle() = 0;
    virtual void tunePidLoop(float kp, float ki, float kd);
};

#endif
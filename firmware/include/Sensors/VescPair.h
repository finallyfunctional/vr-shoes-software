#ifndef VescPair_h
#define VescPair_h

#include <Arduino.h>
#include "../Vector2D.h"
#include "./Sensors/MovementTracker.h"
#include "./Sensors/SpeedController.h"
#include "./Vesc.h"
#include "../Timer.h"

class VescPair : public MovementTracker, public SpeedController
{
    public:
    VescPair(Vesc* forwardVesc, Vesc* sidewaysVesc);
    void update();
    Vector2D getSpeed();
    Vector2D getDistanceFromOrigin();
    void resetOrigin();
    void setRpm(float forwardRpm, float sidewayRpm);
    void inverseForwardDirection();
    void inverseSidewayDirection();
    void resetDirections();

    private:
    Vesc* forwardVesc;
    Vesc* sidewaysVesc;
    Timer* updateTimer;
};

#endif
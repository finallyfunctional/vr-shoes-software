#ifndef VescPair_h
#define VescPair_h

#include <Arduino.h>
#include "../Vector2D.h"
#include "./Sensors/MovementTracker.h"
#include "./Vesc.h"

class VescPair : public MovementTracker
{
    public:
    VescPair(Vesc* forwardVesc, Vesc* sidewaysVesc);
    void update();
    Vector2D getSpeed();
    Vector2D getDistanceFromOrigin();
    void resetOrigin();

    private:

    Vesc* forwardVesc;
    Vesc* sidewaysVesc;
};

#endif
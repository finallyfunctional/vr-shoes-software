#ifndef MovementTracker_h
#define MovementTracker_h

#include <Arduino.h>
#include "../Vector2D.h"

class MovementTracker
{
    public:
    virtual void update() = 0;
    virtual void resetOrigin() = 0;
    virtual Vector2D getSpeed() = 0;
    virtual Vector2D getDistanceFromOrigin() = 0;
    virtual float getOrientation() = 0;
};

#endif
#ifndef MovementTracker_h
#define MovementTracker_h

#include <Arduino.h>
#include "../Vector2D.h"

class MovementTracker
{
    public:
    virtual void update() = 0;
    virtual void resetDistance() = 0;
    virtual Vector2D getSpeed() = 0;
    virtual Vector2D getDistanceTracked() = 0;
    virtual Vector2D getDesiredSpeed() = 0;
};

#endif
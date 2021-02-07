#ifndef StrideTracker_h
#define StrideTracker_h

#include <Arduino.h>
#include "Range.h"
#include "../States/ShoePositionState.h"
#include "../../Sensors/MovementTracker.h"


class StrideTracker
{
    public:
    StrideTracker(MovementTracker* movementTracker);
    void reset();
    int getPosition();
    void storeCurrentStrideLength();

    private:
    float getAverageStrideLength();
    void clearStrideLengths();

    const static int NUM_STRIDES = 20;
    const float APPROX_CENTER_RADIUS = 0.15;
    const float DEFAULT_STRIDE_LENGTH = 0.748;

    MovementTracker* movementTracker;
    Range* centerRange;
    float recentStrideLengths[NUM_STRIDES]; //keep track of last 20 stride lengths and average them
    bool startedAtCenter;
    int strideLengthIndex;
};

#endif
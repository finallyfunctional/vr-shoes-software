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
    void recenter();
    int getPosition();
    void updateCurrentStrideLength();
    float getCenterRadius();
    void setCenterRadius(float centerRadius);
    float getStrideLength();
    void shouldCalculateStrideLength(bool calculate);
    bool isStrideLengthCalculated();
    float getCenterOffset();
    void setCenterOffset(float offset);

    private:

    const float DEFAULT_CENTER_RADUIS = 0.1;
    const float DEFAULT_STRIDE_LENGTH = 0.45;
    const float DEFAULT_CENTER_OFFSET = 0;

    MovementTracker* movementTracker;
    Range* centerRange;
    float strideLength;
    float centerRadius;
    float centerOffset;
    bool calculateStrideLength;
    bool startedAtCenter;
};

#endif
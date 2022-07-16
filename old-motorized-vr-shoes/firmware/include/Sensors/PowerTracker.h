#ifndef PowerTracker_h
#define PowerTracker_h

#include <Arduino.h>
#include "./PowerStatistics.h"

class PowerTracker 
{
    public:
    virtual PowerStatistics getForwardCurrentStatistics() = 0;
    virtual PowerStatistics getSidewayCurrentStatistics() = 0;
};

#endif
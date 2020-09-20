#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor
{
    public:
    static const int POLES;
    static const float GEARING_RATIO;
    static const int MAX_RPM;
};

#endif
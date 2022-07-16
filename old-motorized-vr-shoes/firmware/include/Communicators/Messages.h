#ifndef Messages_h
#define Messages_h

#include <Arduino.h>

class Messages
{
    public: 
    static const String PING;
    static const String SET_COMMUNICATION_MODE;
    static const String SENSOR_DATA;
    static const String RESET_DISTANCE_TRACKER;
    static const String SET_RPM;
    static const String SHOE_CONFIGURATIONS;
    static const String START_NEGATING_MOTION;
    static const String STOP_NEGATING_MOTION;
    static const String POWER_STATISTICS;
};

#endif
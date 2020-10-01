#ifndef Messages_h
#define Messages_h

#include <Arduino.h>

class Messages
{
    public: 
    static const String PING;
    static const String SET_COMMUNICATION_MODE;
    static const String SENSOR_DATA;
    static const String RESET_ORIGIN;
    static const String SET_RPM;
    static const String OTHER_SHOE_ID;
    static const String SHOE_SIDE;
    static const String START_ALGORITHM;
    static const String STOP_ALGORITHM;
    static const String EXTRA_SENSOR_DATA;
    static const String DUTY_CYCLE_BOOST;
    static const String TUNE_PID_LOOP;
    static const String SPEED_MULTIPLIER;
    static const String POWER_STATISTICS;
};

#endif
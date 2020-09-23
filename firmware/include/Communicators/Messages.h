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
    static const String DISTANCE_FROM_ORIGIN;
    static const String SET_RPM;
    static const String OTHER_SHOE_ID;
    static const String SHOE_SIDE;
};

#endif
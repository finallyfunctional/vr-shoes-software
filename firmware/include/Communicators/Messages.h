#ifndef Messages_h
#define Messages_h

#include <Arduino.h>

class Messages
{
    public: 
    static const String PING;
    static const String READ_SENSOR_DATA;
    static const String RESET_ORIGIN;
    static const String READ_DISTANCE_FROM_ORIGIN;

    static const String DEVICE_ID;
};

#endif
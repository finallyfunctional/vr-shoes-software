#ifndef MessageKeys_h
#define MessageKeys_h

#include <Arduino.h>

class MessageKeys
{
    public:
    static const String SHOE_ID;
    static const String COMMAND;
    static const String REPLY;
    static const String DESTINATION;
    static const String FRONT_BUTTON_PRESSED;
    static const String REAR_BUTTON_PRESSED;
    static const String FORWARD_SPEED;
    static const String SIDEWAY_SPEED;
    static const String FORWARD_DISTANCE;
    static const String SIDEWAY_DISTANCE;
    static const String FORWARD_RPM;
    static const String SIDEWAY_RPM;
    static const String OTHER_SHOE_ID;
    static const String REMOTE_NAME;
    static const String SUCCESS;
    static const String ERROR;
    static const String GET;
    static const String MODE;
    static const String SIDE;
    static const String FORWARD_DESIRED_SPEED;
    static const String SIDEWAY_DESIRED_SPEED;
    static const String FORWARD_DUTY_CYCLE;
    static const String SIDEWAY_DUTY_CYCLE;
    static const String DUTY_CYCLE_BOOST;
};

#endif
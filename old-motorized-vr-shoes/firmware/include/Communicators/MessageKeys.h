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
    static const String KP;
    static const String KI;
    static const String KD;
    static const String SIDEWAY_PEAK_CURRENT;
    static const String SIDEWAY_CURRENT_NOW;
    static const String SIDEWAY_AVERAGE_CURRENT;
    static const String SIDEWAY_AMP_HOURS;
    static const String SIDEWAY_AMP_HOURS_CHARGED;
    static const String FORWARD_PEAK_CURRENT;
    static const String FORWARD_CURRENT_NOW;
    static const String FORWARD_AVERAGE_CURRENT;
    static const String FORWARD_AMP_HOURS;
    static const String FORWARD_AMP_HOURS_CHARGED;
    static const String SPEED_MULTIPLER;
    static const String FRONT_BUTTON_PRESSED_VALUE;
    static const String REAR_BUTTON_PRESSED_VALUE;
    static const String BUTTON_MAX_DIFFERENCE;
    static const String MOVEMENT_STATE;
    static const String STRIDE_LENGTH;
    static const String CENTER_RADIUS;
    static const String CENTER_OFFSET;
    static const String CALCULATE_STRIDE_LENGTH;
    static const String POSITION;
};

#endif
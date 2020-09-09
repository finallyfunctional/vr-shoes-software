#ifndef Communicator_h
#define Communicator_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "./Messages.h"
#include "../Sensors/Sensors.h"
#include "../Vector2D.h"

class Communicator
{
    public:
    void initialize(Preferences preferences, Sensors* sensors);
    void update();

    private:
    static const char* DEVICE_ID_KEY;
    static const String DEVICE_ID_PREFIX;

    StaticJsonDocument<200> json;
    Sensors* sensors;

    protected:
    static const char MESSAGE_TERMINATOR;

    virtual void initializeCommunication() = 0;
    virtual void processMessages() = 0;
    void handleMessage(String message);
    virtual void sendMessage(String message) = 0;
    void ping();
    void sendSensorData(bool force);
    void resetOrigin();
    void sendDistanceFromOrigin();

    String deviceId;
    String lastSensorDataMessageSent;
};

#endif
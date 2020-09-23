#ifndef Communicator_h
#define Communicator_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "./Messages.h"
#include "../Sensors/Sensors.h"
#include "../Vector2D.h"
#include "../VrShoeGlobals.h"
#include "./ResponseCodes.h"
#include "./MessageKeys.h"
#include "../Timer.h"
#include "../ShoeSides.h"

class Communicator
{
    public:
    void initialize(Sensors* sensors);
    void update();

    private:
    int ping();
    int replyWithSensorData();
    void sendSensorDataIfStale(String sendingToDeviceId);
    void getSensorData();
    int recieveSensorData();
    int resetOrigin();
    int sendDistanceFromOrigin();
    int setRpm();
    int setOtherShoeId();
    int setShoeSide();
    int getShoeSide();
    float roundFloatToTwoDecimalPlaces(float number);

    static const char* SHOE_ID_KEY;
    static const String DEVICE_ID_PREFIX;

    protected:

    Sensors* sensors;
    Timer* sendSensorDataTimer;

    static const char MESSAGE_TERMINATOR;

    virtual void initializeCommunication() = 0;
    virtual void processMessages() = 0;
    virtual int handleImplementationSpecificMessage(String commandId) = 0;
    virtual int setCommunicationMode() = 0;
    virtual bool isInitialized() = 0;
    virtual bool shouldForwardMessage() = 0;
    virtual void sendMessageTo(String sendingToDeviceId) = 0;
    int handleRecievedMessage(String message);

    String shoeId;
    String lastSensorDataMessageSent;
    StaticJsonDocument<200> json;
};

#endif
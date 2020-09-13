#ifndef VrShoeConfiguration_h
#define VrShoeConfiguration_h

#include <Arduino.h>
#include <Preferences.h>
#include "./Communicators/Communicator.h"
#include "./Communicators/Bluetooth/BluetoothCommunicator.h"
#include "./Communicators/CommunicatorTypes.h"
#include "./Sensors/Buttons/Button.h"
#include "./Sensors/Buttons/ButtonTypes.h"
#include "./Sensors/Buttons/AdafruitMiniSoftTouchButton.h"
#include "./Sensors/Sensors.h"
#include "./Sensors/VescPair.h"
#include "./Sensors/Vesc.h"
#include "./Wheels/Rotacaster50mmWheel.h"
#include "./Motors/Flipsky270kvMotor.h"

class VrShoeConfiguration
{
    public:
    void initialize(Preferences preferences);
    Communicator* getCommunicator();
    Sensors* getSensors();

    private:
    Communicator* communicator;
    Sensors* sensors;

    static const char* COMMUNICATOR_TYPE_KEY;
    static const char* BUTTONS_TYPE_KEY;
    static const int FRONT_BUTTON_PIN = 5;
    static const int REAR_BUTTON_PIN = 4;

};

#endif
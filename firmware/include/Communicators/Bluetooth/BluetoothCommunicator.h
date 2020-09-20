#ifndef BluetoothCommunicator_h
#define BluetoothCommunicator_h

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "../Communicator.h"
#include "../../VrShoeGlobals.h"
#include "esp_bt.h"
#include "../ResponseCodes.h"
#include "../MessageKeys.h"
#include "./BluetoothMessages.h"

extern Preferences preferences;

class BluetoothCommunicator : public Communicator
{
    public:
    void initializeCommunication();
    void processMessages();

    private:
    BluetoothSerial serialBt;
    bool btReady;
    bool connected;
    bool isMaster;

    static const char* OTHER_SHOE_ID_KEY;
    static const char* BT_MODE_KEY;
    static const String BLUETOOTH_MASTER_MODE;
    static const String BLUETOOTH_SLAVE_MODE;
    static const int MAX_MESSAGES_PER_LOOP = 1;

    void endBtSerial();

    protected:
    void sendMessageTo(String sendingToDeviceId);
    int setCommunicationMode();
    int handleImplementationSpecificMessage(String commandId);
    bool isInitialized();
    bool shouldForwardMessage();
};

#endif
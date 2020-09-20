#include "../../../include/Communicators/Bluetooth/BluetoothCommunicator.h"

const String BluetoothCommunicator::BLUETOOTH_MASTER_MODE  = "btMasterMode";
const String BluetoothCommunicator::BLUETOOTH_SLAVE_MODE  = "btSlaveMode";
const char* BluetoothCommunicator::OTHER_SHOE_ID_KEY = "remote-id";
const char* BluetoothCommunicator::BT_MODE_KEY = "bt-mode";

void BluetoothCommunicator::initializeCommunication()
{
    String mode = VrShoePreferences.getString(BT_MODE_KEY, "");
    String otherShoeId = VrShoePreferences.getString(OTHER_SHOE_ID_KEY, "");
    if(mode.equals("") || otherShoeId.equals(""))
    {
        //start in slave mode
        serialBt.begin(shoeId);
        btReady = true;
        connected = true;
        isMaster = false;
        Serial.println("Starting in bluetooth slave mode");
    }
    else
    {
        //start in master mode
        serialBt.begin(shoeId, true);
        btReady = true;
        connected = serialBt.connect(otherShoeId);
        isMaster = true;
        if(!connected)
        {
            Serial.print("Could not connect to other shoe with name: ");
            Serial.println(otherShoeId);
        }
        else 
        {
            Serial.println("Starting in bluetooth master mode");
            Serial.print("Connected to other shoe: ");
            Serial.println(otherShoeId);
        }
    }
}

int BluetoothCommunicator::setCommunicationMode()
{
    String mode = json["mode"].as<String>();
    Serial.print("Mode: ");
    Serial.println(mode);
    if(mode.equals(BLUETOOTH_MASTER_MODE))
    {     
        Serial.println("Changing to master mode");
        String otherShoeId = json["otherShoeId"].as<String>();
        Serial.print("Other Shoe ID: ");
        Serial.println(otherShoeId);
        VrShoePreferences.putString(OTHER_SHOE_ID_KEY, otherShoeId);
        VrShoePreferences.putString(BT_MODE_KEY, BLUETOOTH_MASTER_MODE);
        endBtSerial();
        btReady = false;
    }
    else if(mode.equals(BLUETOOTH_SLAVE_MODE))
    {
        VrShoePreferences.putString(BT_MODE_KEY, BLUETOOTH_SLAVE_MODE);
        VrShoePreferences.remove(OTHER_SHOE_ID_KEY);
        endBtSerial();
        btReady = false;
    }
    return ResponseCodes::GOOD_REQUEST_NO_REPLY;
}

void BluetoothCommunicator::endBtSerial()
{
    serialBt.end();
    /**
     * I was getting the same error as here - https://github.com/espressif/arduino-esp32/issues/2718
     * Even though I have the latest version, which should include the fix from that issue, 
     * the error still happens. So I added the fix manually here.
    **/
    esp_bt_controller_deinit();
}

bool BluetoothCommunicator::isInitialized()
{
    return btReady && connected;
}

void BluetoothCommunicator::processMessages()
{
    int messagesProcessed = 0;
    while(serialBt.available() && messagesProcessed < MAX_MESSAGES_PER_LOOP)
    {
        String message = serialBt.readStringUntil(MESSAGE_TERMINATOR);
        int response = handleRecievedMessage(message);
        if(response == ResponseCodes::GOOD_REQUEST_SEND_REPLY || 
           response == ResponseCodes::BAD_REQUEST)
        {
            sendMessageTo("");
        }
        else if(response == ResponseCodes::GOOD_REQUEST_NO_REPLY)
        {
            //no reply needed, do nothing
        }
        else if(response == ResponseCodes::FORWARD_MESSAGE)
        {
            String deviceToSendTo = json[MessageKeys::SHOE_ID];
            sendMessageTo(deviceToSendTo);
        }
        else
        {
            Serial.print("Unknown response - ");
            Serial.println(response);
        }
        messagesProcessed++;
    }
}

bool BluetoothCommunicator::shouldForwardMessage()
{
    return !isMaster && 
    (json["reply"].as<bool>() || 
    (!json["to"].isNull() && !json["to"].as<String>().equals(shoeId)));
}

int BluetoothCommunicator::handleImplementationSpecificMessage(String commandId)
{
    if(isMaster && commandId.equals(BluetoothMessages::BT_CONNECT_TO))
    {
        String remoteName = json["remoteName"];
        json.clear();
        json["command"] = BluetoothMessages::BT_CONNECT_TO;
        json["reply"] = true;
        json[MessageKeys::SHOE_ID] = shoeId;
        json["success"] = serialBt.connect(remoteName);
        return ResponseCodes::GOOD_REQUEST_SEND_REPLY;
    }
    return ResponseCodes::BAD_REQUEST;
}

//sendingToshoeId doesn't matter with BluetoothSerial.
void BluetoothCommunicator::sendMessageTo(String sendingToshoeId)
{
    String message;
    serializeJson(json, message);
    serialBt.println(message);
}
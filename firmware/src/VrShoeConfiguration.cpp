#include "../include/VrShoeConfiguration.h"

const char* VrShoeConfiguration::COMMUNICATOR_TYPE_KEY = "comm-type";
const char* VrShoeConfiguration::BUTTONS_TYPE_KEY = "btns-type";

void VrShoeConfiguration::initialize(Preferences preferences)
{
    String communicationType = preferences.getString(COMMUNICATOR_TYPE_KEY);
    if(communicationType == NULL || communicationType == "")
    {
        preferences.putString(COMMUNICATOR_TYPE_KEY, CommunicatorTypes::BLUETOOTH);
    }
    communicator = new BluetoothCommunicator();
    
    String buttonType = preferences.getString(BUTTONS_TYPE_KEY);
    if(buttonType == NULL || buttonType == "")
    {
        preferences.putString(BUTTONS_TYPE_KEY, ButtonTypes::ADAFRUIT_MINI_SOFT_TOUCH);
    }
    Button* frontButton = new AdafruitMiniSoftTouchButton(FRONT_BUTTON_PIN);
    Button* rearButton = new AdafruitMiniSoftTouchButton(REAR_BUTTON_PIN);
    frontButton->initialize();
    rearButton->initialize();

    Vesc* forwardVesc = new Vesc(&Serial2, Flipsky270kvMotor(), Rotacaster50mmWheel());
    Vesc* sidewaysVesc = new Vesc(&Serial1, Flipsky270kvMotor(), Rotacaster50mmWheel());
    VescPair* vescPair = new VescPair(forwardVesc, sidewaysVesc);

    sensors = new Sensors(frontButton, rearButton, vescPair);

    communicator->initialize(preferences, sensors);
}

Communicator* VrShoeConfiguration::getCommunicator()
{
    return communicator;
}

Sensors* VrShoeConfiguration::getSensors()
{
    return sensors;
}
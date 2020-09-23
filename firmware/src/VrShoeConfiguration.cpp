#include "../include/VrShoeConfiguration.h"

const char* VrShoeConfiguration::COMMUNICATOR_TYPE_KEY = "commType";
const char* VrShoeConfiguration::BUTTONS_TYPE_KEY = "btnsType";

void VrShoeConfiguration::initialize()
{
    String communicationType = VrShoePreferences.getString(COMMUNICATOR_TYPE_KEY);
    if(communicationType == NULL || communicationType == "")
    {
        VrShoePreferences.putString(COMMUNICATOR_TYPE_KEY, CommunicatorTypes::BLUETOOTH);
    }
    communicator = new BluetoothCommunicator();
    
    String buttonType = VrShoePreferences.getString(BUTTONS_TYPE_KEY);
    if(buttonType == NULL || buttonType == "")
    {
        VrShoePreferences.putString(BUTTONS_TYPE_KEY, ButtonTypes::ADAFRUIT_MINI_SOFT_TOUCH);
    }
    Button* frontButton = new AdafruitMiniSoftTouchButton(FRONT_BUTTON_PIN);
    Button* rearButton = new AdafruitMiniSoftTouchButton(REAR_BUTTON_PIN);
    frontButton->initialize();
    rearButton->initialize();

    Vesc* forwardVesc = new Vesc(&Serial2, Flipsky270kvMotor(), Rotacaster50mmWheel());
    Vesc* sidewaysVesc = new Vesc(&Serial1, Flipsky270kvMotor(), Rotacaster50mmWheel());
    VescPair* vescPair = new VescPair(forwardVesc, sidewaysVesc);

    int shoeSide = VrShoePreferences.getInt(ShoeSides::SHOE_SIDE_KEY, ShoeSides::RIGHT);
    if(shoeSide == ShoeSides::LEFT)
    {
        vescPair->inverseForwardDirection();
    }

    sensors = new Sensors(frontButton, rearButton, vescPair, vescPair, new RemoteVrShoe());

    shoeController = new OmniPoweredWalking(sensors, communicator);

    communicator->initialize(sensors);
}

Communicator* VrShoeConfiguration::getCommunicator()
{
    return communicator;
}

Sensors* VrShoeConfiguration::getSensors()
{
    return sensors;
}

AutoShoeController* VrShoeConfiguration::getShoeController()
{
    return shoeController;
}
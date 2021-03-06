#include "../include/VrShoeConfiguration.h"

const char* VrShoeConfiguration::COMMUNICATOR_TYPE_KEY = "commType";

void VrShoeConfiguration::initialize()
{
    String communicationType = VrShoePreferences.getString(COMMUNICATOR_TYPE_KEY);
    if(communicationType == NULL || communicationType == "")
    {
        VrShoePreferences.putString(COMMUNICATOR_TYPE_KEY, CommunicatorTypes::BLUETOOTH);
    }
    communicator = new BluetoothCommunicator();
    
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

    sensors = new Sensors(frontButton, rearButton, vescPair, vescPair, new RemoteVrShoe(), vescPair);

    shoeController = new ForwardWalking(sensors, shoeSide);

    communicator->initialize(sensors, shoeController);
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
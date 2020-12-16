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
    
    int frontButtonPressedValue = VrShoePreferences.getInt(Button::FRONT_BUTTON_PRESSED_VALUE_KEY, 0);
    int rearButtonPressedValue = VrShoePreferences.getInt(Button::REAR_BUTTON_PRESSED_VALUE_KEY, 0);
    int buttonMaxDiff = VrShoePreferences.getInt(Button::MAX_DIFF_VALUE_KEY, 0);
    Button* frontButton = new LinearHallEffectSensorButton(FRONT_BUTTON_PIN, frontButtonPressedValue);
    Button* rearButton = new LinearHallEffectSensorButton(REAR_BUTTON_PIN, rearButtonPressedValue);
    frontButton->initialize();
    rearButton->initialize();
    frontButton->setMaxDifferencePercentage(buttonMaxDiff);
    rearButton->setMaxDifferencePercentage(buttonMaxDiff);

    Vesc* forwardVesc = new Vesc(&Serial2, Flipsky270kvMotor(), Rotacaster50mmWheel());
    Vesc* sidewaysVesc = new Vesc(&Serial1, Flipsky270kvMotor(), Rotacaster50mmWheel());
    VescPair* vescPair = new VescPair(forwardVesc, sidewaysVesc);

    int shoeSide = VrShoePreferences.getInt(ShoeSides::SHOE_SIDE_KEY, ShoeSides::RIGHT);
    if(shoeSide == ShoeSides::LEFT)
    {
        vescPair->inverseForwardDirection();
    }

    sensors = new Sensors(frontButton, rearButton, vescPair, vescPair, new RemoteVrShoe(), vescPair);

    shoeController = new OmniPoweredWalking(sensors, shoeSide);

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
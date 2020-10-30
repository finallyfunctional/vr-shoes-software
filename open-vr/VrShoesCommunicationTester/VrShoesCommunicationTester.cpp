#include <CommunicationInitializer.h>
#include <BtCommunicationInitializer.h>
#include <Communicator.h>
#include <VrShoe.h>
#include <thread>
#include <chrono>

void printVrShoeStateChanges(VrShoe prevVrShoe, VrShoe vrShoe)
{
    if (vrShoe.forwardSpeed != prevVrShoe.forwardSpeed)
    {
        printf("%s forward speed - %f m/s \r\n", vrShoe.deviceId, vrShoe.forwardSpeed);
    }
    if (vrShoe.frontButtonPressed != prevVrShoe.frontButtonPressed)
    {
        if (vrShoe.frontButtonPressed)
        {
            printf("%s front button pressed\r\n", vrShoe.deviceId);
        }
        else
        {
            printf("%s front button unpressed\r\n", vrShoe.deviceId);
        }
    }
    if (vrShoe.rearButtonPressed != prevVrShoe.rearButtonPressed)
    {
        if (vrShoe.rearButtonPressed)
        {
            printf("%s rear button pressed\r\n", vrShoe.deviceId);
        }
        else
        {
            printf("%s rear button unpressed\r\n", vrShoe.deviceId);
        }
    }
}

void copyValues(VrShoe* source, VrShoe* destination)
{
    destination->deviceId = source->deviceId;
    destination->forwardSpeed = source->forwardSpeed;
    destination->frontButtonPressed = source->frontButtonPressed;
    destination->rearButtonPressed = source->rearButtonPressed;
}

int main()
{
    CommunicationInitializer* communicationInitializer = new BtCommunicationInitializer();
    if (!communicationInitializer->initialize())
    {
        printf("There was an error setting up BT communication\r\n");
        return 0;
    }
    printf("BT communication initialized\r\n");
    VrShoe* vrShoe1 = communicationInitializer->getVrShoe1();
    VrShoe* vrShoe2 = communicationInitializer->getVrShoe2();
    Communicator* communicator = communicationInitializer->getCommunicator();
    VrShoe prevVrShoe1 = VrShoe();
    VrShoe prevVrShoe2 = VrShoe();

    while (true)
    {
        communicator->processMessages();

        printVrShoeStateChanges(prevVrShoe1, *vrShoe1);
        printVrShoeStateChanges(prevVrShoe2, *vrShoe2);

        copyValues(vrShoe1, &prevVrShoe1);
        copyValues(vrShoe2, &prevVrShoe2);
    }
}

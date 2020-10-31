#include <BtCommunicator.h>

void BtCommunicator::initializeBt(VrShoe* vrShoe1, VrShoe* vrShoe2, SOCKET vrShoe1BtSocket, SOCKET vrShoe2BtSocket)
{
    initialize(vrShoe1, vrShoe2);
    this->vrShoe1BtSocket = vrShoe1BtSocket;
    this->vrShoe2BtSocket = vrShoe2BtSocket;
    vrShoe1BtReader = new BtMessageReader(vrShoe1BtSocket);
    vrShoe2BtReader = new BtMessageReader(vrShoe2BtSocket);
    initialized = true;
}

void BtCommunicator::processMessages()
{
    if (!initialized)
    {
        return;
    }
    processMessagesForShoe(vrShoe1BtReader);
    processMessagesForShoe(vrShoe2BtReader);
}

void BtCommunicator::processMessagesForShoe(BtMessageReader* btReader)
{
    btReader->processMessagesFromSocket();
    for (int i = 0; i < NUM_MESSAGES_PER_SHOE_PER_LOOP; i++)
    {
        const char* message = btReader->readNextMessage();
        if (message == nullptr)
        {
            return;
        }
        handleRecievedMessage(message);
    }
}

void BtCommunicator::sendMessageTo(const char* sendingToDeviceId, const char* message)
{
    if (!initialized)
    {
        return;
    }
    SOCKET btSocket;
    if (strcmp(sendingToDeviceId, vrShoe1->deviceId) == 0)
    {
        btSocket = vrShoe1BtSocket;
    }
    else if (strcmp(sendingToDeviceId, vrShoe2->deviceId) == 0)
    {
        btSocket = vrShoe2BtSocket;
    }
    else
    {
        return;
    }
    int messageLength = strlen(message);
    int messageWithTerminatorLength = messageLength + 2;
    char* messageWithTerminator = new char[messageWithTerminatorLength];
    strcpy_s(messageWithTerminator, messageWithTerminatorLength, message);
    messageWithTerminator[messageLength] = '\n';
    messageWithTerminator[messageLength + 1] = '\0';
    int sendResult = send(btSocket, messageWithTerminator, messageWithTerminatorLength, 0);
    if (sendResult == SOCKET_ERROR)
    {
        wprintf(L"Sending BT message failed. Error code %d\r\n", WSAGetLastError());
    }
}

bool BtCommunicator::shouldForwardMessage()
{
	return true;
}
#include <BtMessageReader.h>

BtMessageReader::BtMessageReader(SOCKET btSocket) : btSocket(btSocket)
{

}

void BtMessageReader::processMessagesFromSocket()
{
	int recieveResult = recv(btSocket, recieveBuffer, MAX_MESSAGE_LENGTH, 0);
	if (recieveResult < 0)
	{
		return;
	}
	for (int i = 0; i < recieveResult; i++)
	{
		recievedMessage[messageIndex++] = recieveBuffer[i];
		if (recieveBuffer[i] == MESSAGE_TERMINATOR)
		{
			char* message = new char[messageIndex + 1];
			for (int j = 0; j < messageIndex + 1; j++)
			{
				message[j] = recievedMessage[j];
			}
			message[messageIndex] = '\0';
			messageQueue.push(message);
			messageIndex = 0;
		}
	}
}

const char* BtMessageReader::readNextMessage()
{
	if (messageQueue.empty())
	{
		return nullptr;
	}
	const char* message = messageQueue.front();
	messageQueue.pop();
	return message;
}
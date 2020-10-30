#pragma once

#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>
#include <queue>

class BtMessageReader
{
public:
	BtMessageReader(SOCKET btSocket);
	void processMessagesFromSocket();
	const char* readNextMessage();

private:

	static const int MAX_MESSAGE_LENGTH = 512;
	static const char MESSAGE_TERMINATOR = '\n';

	char recieveBuffer[MAX_MESSAGE_LENGTH];
	char recievedMessage[MAX_MESSAGE_LENGTH];
	SOCKET btSocket;
	int messageIndex;

	std::queue<const char*> messageQueue;
};
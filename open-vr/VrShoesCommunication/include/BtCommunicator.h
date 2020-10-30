#pragma once

#include <Communicator.h>
#include <BtMessageReader.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>s
#include <Winsock2.h>
#include <Ws2bth.h>
#include <BluetoothAPIs.h>

class BtCommunicator : public Communicator
{
public:
	void initializeBt(VrShoe* vrShoe1, VrShoe* vrShoe2, SOCKET vrShoe1BtSocket, SOCKET vrShoe2BtSocket);
	void processMessages();

protected:
	void sendMessageTo(const char* sendingToDeviceId, const char* message);
	bool shouldForwardMessage();

private:
	void processMessagesForShoe(BtMessageReader* btReader);

	BtMessageReader* vrShoe1BtReader;
	BtMessageReader* vrShoe2BtReader;
	SOCKET vrShoe1BtSocket;
	SOCKET vrShoe2BtSocket;
	bool initialized;

	static const int NUM_MESSAGES_PER_SHOE_PER_LOOP = 5;
};
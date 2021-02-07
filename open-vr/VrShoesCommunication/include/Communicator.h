#pragma once

#include <VrShoesCommunication.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <ResponseCodes.h>
#include <MessageKeys.h>
#include <Messages.h>
#include <VrShoe.h>

using namespace rapidjson;

class VRSHOESCOMMUNICATION_API Communicator
{
public:
	virtual void processMessages() = 0;
	void getShoeConfigurations(const char* shoeId);
	void startNegatingMovement(const char* shoeId);
	void stopNegatingMovement(const char* shoeId);

protected:
	void initialize(VrShoe* vrShoe1, VrShoe* vrShoe2);
	int handleRecievedMessage(const char* message);

	virtual bool shouldForwardMessage() = 0;
	virtual void sendMessageTo(const char* sendingToDeviceId, const char* message) = 0;

	Document json;
	VrShoe* vrShoe1;
	VrShoe* vrShoe2;

private:
	void recieveSensorData(VrShoe* vrShoe);
	void recieveShoeConfigurations(VrShoe* vrShoe);
	void sendShoeConfigurations(const char* vrShoeId, const char* otherVrShoeId);
};
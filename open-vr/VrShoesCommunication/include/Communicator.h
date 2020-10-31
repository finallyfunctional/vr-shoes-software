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
	void setOtherShoeId(const char* idOfShoeToSendTo, const char* otherShoeId);
	void setDutyCycleBoost(const char* shoeId, float boost);
	void setSpeedMultipler(const char* shoeId, float multiplier);
	void startAlgorithm(const char* shoeId);
	void stopAlgorithm(const char* shoeId);

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
};
#include <Communicator.h>

void Communicator::initialize(VrShoe* vrShoe1, VrShoe* vrShoe2)
{
	this->vrShoe1 = vrShoe1;
	this->vrShoe2 = vrShoe2;
}

int Communicator::handleRecievedMessage(const char* message)
{
	if (json.Parse(message).HasParseError())
	{
		return ResponseCodes::BAD_REQUEST;
	}
	if (!json.HasMember(MessageKeys::SHOE_ID))
	{
		return ResponseCodes::UNKNOWN_REQUEST;
	}
	const char* shoeId = json[MessageKeys::SHOE_ID].GetString();
	VrShoe* vrShoeThatSentMessage;
	VrShoe* otherVrShoe;
	if (strcmp(shoeId, vrShoe1->deviceId) == 0)
	{
		vrShoeThatSentMessage = vrShoe1;
		otherVrShoe = vrShoe2;
	}
	else if (strcmp(shoeId, vrShoe2->deviceId) == 0)
	{
		vrShoeThatSentMessage = vrShoe2;
		otherVrShoe = vrShoe1;
	}
	else
	{
		return ResponseCodes::BAD_REQUEST;
	}

	if (!json.HasMember(MessageKeys::COMMAND))
	{
		return ResponseCodes::UNKNOWN_REQUEST;
	}
	const char* command = json[MessageKeys::COMMAND].GetString();
	if (strcmp(command, Messages::SENSOR_DATA) == 0)
	{
		recieveSensorData(vrShoeThatSentMessage);
		if (shouldForwardMessage())
		{
			sendMessageTo(otherVrShoe->deviceId, message);
		}
		return ResponseCodes::GOOD_REQUEST_NO_REPLY;
	}
	return ResponseCodes::UNKNOWN_REQUEST;
}

void Communicator::recieveSensorData(VrShoe* vrShoe)
{
	vrShoe->frontButtonPressed = json[MessageKeys::FRONT_BUTTON_PRESSED].GetBool();
	vrShoe->rearButtonPressed = json[MessageKeys::REAR_BUTTON_PRESSED].GetBool();
	vrShoe->forwardSpeed = json[MessageKeys::FORWARD_SPEED].GetFloat();
}

void Communicator::setOtherShoeId(const char* idOfShoeToSendTo, const char* otherShoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::OTHER_SHOE_ID);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(idOfShoeToSendTo);
	writer.Key(MessageKeys::OTHER_SHOE_ID);
	writer.String(otherShoeId);
	writer.EndObject();
	sendMessageTo(idOfShoeToSendTo, jsonStr.GetString());
}

void Communicator::setDutyCycleBoost(const char* shoeId, float boost)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::DUTY_CYCLE_BOOST);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.Key(MessageKeys::DUTY_CYCLE_BOOST);
	writer.Double(boost);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}

void Communicator::setSpeedMultipler(const char* shoeId, float multiplier)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::SPEED_MULTIPLIER);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.Key(MessageKeys::SPEED_MULTIPLER);
	writer.Double(multiplier);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}

void Communicator::startAlgorithm(const char* shoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::START_ALGORITHM);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}

void Communicator::stopAlgorithm(const char* shoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::STOP_ALGORITHM);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}
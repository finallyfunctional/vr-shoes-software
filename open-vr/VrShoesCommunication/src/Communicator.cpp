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
	vrShoe->forwardDistance = json[MessageKeys::FORWARD_DISTANCE].GetFloat();
	vrShoe->movementState = json[MessageKeys::MOVEMENT_STATE].GetInt();
}

void Communicator::recieveShoeConfigurations(VrShoe* vrShoe)
{
	vrShoe->otherShoeId = json[MessageKeys::OTHER_SHOE_ID].GetString();
	if (vrShoe->otherShoeId[0] == '\0')
	{
		if (vrShoe == this->vrShoe1)
		{
			sendShoeConfigurations(vrShoe->deviceId, this->vrShoe2->deviceId);
		}
		else
		{
			sendShoeConfigurations(vrShoe->deviceId, this->vrShoe1->deviceId);
		}
	}
}

void Communicator::sendShoeConfigurations(const char* vrShoeId, const char* otherVrShoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::SHOE_CONFIGURATIONS);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(vrShoeId);
	writer.Key(MessageKeys::GET);
	writer.Bool(false);
	writer.Key(MessageKeys::REPLY);
	writer.Bool(false);
	writer.Key(MessageKeys::OTHER_SHOE_ID);
	writer.String(otherVrShoeId);
	writer.Key(MessageKeys::SIDE);
	writer.String(json[MessageKeys::SIDE].GetString());
	writer.Key(MessageKeys::DUTY_CYCLE_BOOST);
	writer.Double(json[MessageKeys::DUTY_CYCLE_BOOST].GetDouble());
	writer.Key(MessageKeys::SPEED_MULTIPLER);
	writer.Double(json[MessageKeys::SPEED_MULTIPLER].GetDouble());
	writer.EndObject();
	sendMessageTo(vrShoeId, jsonStr.GetString());
}

void Communicator::getShoeConfigurations(const char* shoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::SHOE_CONFIGURATIONS);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.Key(MessageKeys::GET);
	writer.Bool(true);
	writer.Key(MessageKeys::REPLY);
	writer.Bool(false);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}

void Communicator::startNegatingMovement(const char* shoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::START_NEGATING_MOTION);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}

void Communicator::stopNegatingMovement(const char* shoeId)
{
	StringBuffer jsonStr;
	Writer<StringBuffer> writer(jsonStr);
	writer.StartObject();
	writer.Key(MessageKeys::COMMAND);
	writer.String(Messages::STOP_NEGATING_MOTION);
	writer.Key(MessageKeys::DESTINATION);
	writer.String(shoeId);
	writer.EndObject();
	sendMessageTo(shoeId, jsonStr.GetString());
}
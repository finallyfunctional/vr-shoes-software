#define CommunicationInitializer_EXPORT

#include <CommunicationInitializer.h>

bool CommunicationInitializer::initialize()
{
	setup();
	if (communicator == nullptr)
	{
		return false;
	}
	communicator->setOtherShoeId(vrShoe1->deviceId, vrShoe2->deviceId);
	communicator->setOtherShoeId(vrShoe2->deviceId, vrShoe1->deviceId);
	communicator->setDutyCycleBoost(vrShoe1->deviceId, 0.2f);
	communicator->setDutyCycleBoost(vrShoe2->deviceId, 0.2f);
	communicator->setSpeedMultipler(vrShoe1->deviceId, 3);
	communicator->setSpeedMultipler(vrShoe2->deviceId, 3);
	return true;
}

Communicator* CommunicationInitializer::getCommunicator()
{
	return communicator;
}

VrShoe* CommunicationInitializer::getVrShoe1()
{
	return vrShoe1;
}

VrShoe* CommunicationInitializer::getVrShoe2()
{
	return vrShoe2;
}
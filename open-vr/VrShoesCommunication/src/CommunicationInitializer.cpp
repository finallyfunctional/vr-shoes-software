#define CommunicationInitializer_EXPORT

#include <CommunicationInitializer.h>

bool CommunicationInitializer::initialize()
{
	setup();
	if (communicator == nullptr)
	{
		return false;
	}
	communicator->getShoeConfigurations(vrShoe1->deviceId);
	communicator->getShoeConfigurations(vrShoe2->deviceId);
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
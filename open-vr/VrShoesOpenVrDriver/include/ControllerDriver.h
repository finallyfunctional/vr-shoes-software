#pragma once

#include <CommunicationInitializer.h>
#include <BtCommunicationInitializer.h>
#include <Communicator.h>
#include <VrShoe.h>
#include <openvr_driver.h>
//Windows.h must always be included last
#include <windows.h>

using namespace vr;

class ControllerDriver : public ITrackedDeviceServerDriver
{
public:

	EVRInitError Activate(uint32_t unObjectId);
	void Deactivate();
	void EnterStandby();
	void LeaveStandby();
	void* GetComponent(const char* pchComponentNameAndVersion);
	void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);
	DriverPose_t GetPose();
	void RunFrame();

private:
	EVRInitError InitializeVrShoeCommunication();
	EVRInitError InitializeOpenVrConfigurations(uint32_t unObjectId);
	float GetYSpeed();

	uint32_t driverId;
	VRInputComponentHandle_t joystickYHandle;
	VRInputComponentHandle_t trackpadYHandle;
	VRInputComponentHandle_t joystickXHandle;
	VRInputComponentHandle_t trackpadXHandle;

	Communicator* communicator;
	VrShoe* vrShoe1;
	VrShoe* vrShoe2;
};
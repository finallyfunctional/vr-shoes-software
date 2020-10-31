#pragma once

#include <CommunicationInitializer.h>
#include <BtCommunicationInitializer.h>
#include <Communicator.h>
#include <VrShoe.h>
#include <openvr_driver.h>
//Windows.h must always be included last
#include <windows.h>

using namespace vr;

/**
Configures the behavior of the device. Sends state changes 
to OpenVR every frame.
**/
class ControllerDriver : public ITrackedDeviceServerDriver
{
public:

	/**
	Initializes this driver. Sets up OpenVR configurations 
	and connects to VR shoes.
	**/
	EVRInitError Activate(uint32_t unObjectId);

	/**
	Un-initialize the controller.
	**/
	void Deactivate();

	/**
	Tells VR shoes to go into standby mode.
	**/
	void EnterStandby();

	/**
	Returns this class as an input device.
	**/
	void* GetComponent(const char* pchComponentNameAndVersion);

	void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);

	/**
	Returns a default Pose. VR shoes do not use Pose.
	**/
	DriverPose_t GetPose();

	/**
	Retrieves VR shoe state information and updates Open VR with any state changes.
	**/
	void RunFrame();

private:

	uint32_t driverId;
	VRInputComponentHandle_t joystickYHandle;
	VRInputComponentHandle_t trackpadYHandle;
	VRInputComponentHandle_t joystickXHandle;
	VRInputComponentHandle_t trackpadXHandle;

};
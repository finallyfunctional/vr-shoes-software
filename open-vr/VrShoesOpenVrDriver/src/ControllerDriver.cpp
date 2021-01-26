#include <ControllerDriver.h>

EVRInitError ControllerDriver::Activate(uint32_t unObjectId)
{
	EVRInitError error = InitializeVrShoeCommunication();
	if (error != VRInitError_None)
	{
		return error;
	}
	return InitializeOpenVrConfigurations(unObjectId);
}

EVRInitError ControllerDriver::InitializeVrShoeCommunication()
{
	CommunicationInitializer* communicationInitializer = new BtCommunicationInitializer();
	if (!communicationInitializer->initialize())
	{
		VRDriverLog()->Log("There was an error setting up BT communication");
		return VRInitError_Unknown;
	}
	VRDriverLog()->Log("BT communication initialized");
	vrShoe1 = communicationInitializer->getVrShoe1();
	vrShoe2 = communicationInitializer->getVrShoe2();
	communicator = communicationInitializer->getCommunicator();
	return VRInitError_None;
}

EVRInitError ControllerDriver::InitializeOpenVrConfigurations(uint32_t unObjectId)
{
	driverId = unObjectId;

	PropertyContainerHandle_t props = VRProperties()->TrackedDeviceToPropertyContainer(driverId);

	VRProperties()->SetStringProperty(props, Prop_InputProfilePath_String, "{ffVrShoes}/input/ffVrShoes_profile.json");
	VRProperties()->SetInt32Property(props, Prop_ControllerRoleHint_Int32, ETrackedControllerRole::TrackedControllerRole_Treadmill);
	VRProperties()->SetStringProperty(props, Prop_SerialNumber_String, "ffVrShoes");
	VRDriverInput()->CreateScalarComponent(props, "/input/joystick/y", &joystickYHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
	VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/y", &trackpadYHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
	VRDriverInput()->CreateScalarComponent(props, "/input/joystick/x", &joystickXHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);
	VRDriverInput()->CreateScalarComponent(props, "/input/trackpad/x", &trackpadXHandle, EVRScalarType::VRScalarType_Absolute,
		EVRScalarUnits::VRScalarUnits_NormalizedTwoSided);

	return VRInitError_None;
}

DriverPose_t ControllerDriver::GetPose()
{
	DriverPose_t pose = { 0 };
	pose.poseIsValid = false;
	pose.result = TrackingResult_Calibrating_OutOfRange;
	pose.deviceIsConnected = true;

	HmdQuaternion_t quat;
	quat.w = 1;
	quat.x = 0;
	quat.y = 0;
	quat.z = 0;

	pose.qWorldFromDriverRotation = quat;
	pose.qDriverFromHeadRotation = quat;

	return pose;
}

//Y is forward/backward, X is sideways
void ControllerDriver::RunFrame()
{
	communicator->processMessages();

	float ySpeed = GetYSpeed();

	VRDriverInput()->UpdateScalarComponent(joystickYHandle, ySpeed, 0);
	VRDriverInput()->UpdateScalarComponent(trackpadYHandle, ySpeed, 0);

	VRDriverInput()->UpdateScalarComponent(joystickXHandle, 0.0f, 0);
	VRDriverInput()->UpdateScalarComponent(trackpadXHandle, 0.0f, 0);
}

float ControllerDriver::GetYSpeed()
{
	VrShoe vrShoeWithFootInAir;
	if (!vrShoe1->frontButtonPressed && !vrShoe1->rearButtonPressed)
	{
		vrShoeWithFootInAir = *vrShoe1;
		VRDriverLog()->Log("VR Shoe 1 is in the air");
		if (!vrShoe2->frontButtonPressed && !vrShoe2->rearButtonPressed)
		{
			VRDriverLog()->Log("VR Shoe 2 is also in the air");
			return 0;
		}
	}
	else if (!vrShoe2->frontButtonPressed && !vrShoe2->rearButtonPressed)
	{
		vrShoeWithFootInAir = *vrShoe2;
		VRDriverLog()->Log("VR Shoe 2 is in the air");
		if (!vrShoe1->frontButtonPressed && !vrShoe1->rearButtonPressed)
		{
			VRDriverLog()->Log("VR Shoe 1 is also in the air");
			return 0;
		}
	}
	else
	{
		VRDriverLog()->Log("Niether foot was in the air");
		return 0;
	}
	if (vrShoeWithFootInAir.forwardDistanceFromOrigin <= 0)
	{
		VRDriverLog()->Log("Foot in the air distance is 0");
		return 0;
	}
	VRDriverLog()->Log("Walking! Sending 1");
	return 1;
}

void ControllerDriver::Deactivate()
{
	driverId = k_unTrackedDeviceIndexInvalid;
}

void* ControllerDriver::GetComponent(const char* pchComponentNameAndVersion)
{
	if (strcmp(IVRDriverInput_Version, pchComponentNameAndVersion) == 0)
	{
		return this;
	}
	return NULL;
}

void ControllerDriver::EnterStandby() 
{
	communicator->stopAlgorithm(vrShoe1->deviceId);
	communicator->stopAlgorithm(vrShoe2->deviceId);
}

void ControllerDriver::LeaveStandby()
{
	communicator->startAlgorithm(vrShoe1->deviceId);
	communicator->startAlgorithm(vrShoe2->deviceId);
}

void ControllerDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) 
{
	if (unResponseBufferSize >= 1)
	{
		pchResponseBuffer[0] = 0;
	}
}
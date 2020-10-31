#include <ControllerDriver.h>

const float ControllerDriver::MIN_SHOE_ABS_SPEED = 0.1f;
const float ControllerDriver::Y_JOYSTICK_ABS_SPEED = 1.0f;

EVRInitError ControllerDriver::Activate(uint32_t unObjectId)
{
	InitializeVrShoeCommunication();
	InitializeOpenVrConfigurations(unObjectId);
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
	
	float ySpeed = GetYSpeed(vrShoe1);
	if (ySpeed == 0)
	{
		ySpeed = GetYSpeed(vrShoe2);
	}

	VRDriverInput()->UpdateScalarComponent(joystickYHandle, ySpeed, 0);
	VRDriverInput()->UpdateScalarComponent(trackpadYHandle, ySpeed, 0);

	VRDriverInput()->UpdateScalarComponent(joystickXHandle, 0.0f, 0);
	VRDriverInput()->UpdateScalarComponent(trackpadXHandle, 0.0f, 0);
}

float ControllerDriver::GetYSpeed(VrShoe* vrShoe)
{
	if (!vrShoe->frontButtonPressed || !vrShoe->rearButtonPressed)
	{
		return 0;
	}
	if (fabs(vrShoe->forwardSpeed) >= MIN_SHOE_ABS_SPEED)
	{
		if (vrShoe->forwardSpeed > 0)
		{
			return Y_JOYSTICK_ABS_SPEED;
		}
		else
		{
			return Y_JOYSTICK_ABS_SPEED * -1;
		}
	}
	return 0;
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
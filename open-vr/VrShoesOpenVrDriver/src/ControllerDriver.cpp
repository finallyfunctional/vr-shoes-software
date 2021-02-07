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

	previousYSpeed = ySpeed;
}

float ControllerDriver::GetYSpeed()
{
	if (vrShoe1->movementState != ShoeMovementState::STOPPED || vrShoe2->movementState != ShoeMovementState::STOPPED)
	{
		return 1;
	}
	if (previousYSpeed == 1)
	{
		clockStart = std::chrono::high_resolution_clock::now();
		return 1;
	}
	double ellapsedTime = GetEllapsedTimeInSeconds();
	if (ellapsedTime <= 0.001)
	{
		return 1;
	}
	double speed = GetDeceleratingSpeed(0, 10);
	if (speed > 1)
	{
		return 1;
	}
	else if (speed < 0)
	{
		return 0;
	}
	return speed;
}

/*
y = (-2^(ax) + (2+b)^a)/( (2+b)^(a) - 1 )
y: Output speed.
x: Elapsed time since stopping.
a: Manipulate the graph's slope to make speed lower linearly or exponentially.
b: Default time to 0 speed is 1. Use this variable to add or decrease the time.
*/
double ControllerDriver::GetDeceleratingSpeed(double timeAdjustment, double slopeAdjustment)
{
	double ellapsedTime = GetEllapsedTimeInSeconds();
	if (slopeAdjustment == 0) //cannot be zero, otherwise denominator is zero
	{
		slopeAdjustment = 0.01;
	}
	return (pow(-2, slopeAdjustment * ellapsedTime) + pow(2 + timeAdjustment, slopeAdjustment)) / (pow(2 + timeAdjustment, slopeAdjustment) - 1);
}

double ControllerDriver::GetEllapsedTimeInSeconds()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	long ellapsedTimeInMilli = std::chrono::duration_cast<std::chrono::milliseconds>(now - clockStart).count();
	return (double)(ellapsedTimeInMilli) / 1000;
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
	communicator->stopNegatingMovement(vrShoe1->deviceId);
	communicator->stopNegatingMovement(vrShoe2->deviceId);
}

void ControllerDriver::LeaveStandby()
{
	communicator->startNegatingMovement(vrShoe1->deviceId);
	communicator->startNegatingMovement(vrShoe2->deviceId);
}

void ControllerDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) 
{
	if (unResponseBufferSize >= 1)
	{
		pchResponseBuffer[0] = 0;
	}
}
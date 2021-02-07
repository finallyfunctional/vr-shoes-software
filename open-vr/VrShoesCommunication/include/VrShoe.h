#pragma once

struct VrShoe
{
	const char* deviceId;
	const char* otherShoeId;
	bool frontButtonPressed;
	bool rearButtonPressed;
	float forwardSpeed;
	float forwardDistance;
	int movementState;
};
#pragma once
#include "Object.h"
#include "Camera.h"

bool WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait);

void SignalTheDataHasBeenSubmitted();

void SubmitObjectData(Object * obj);
void SubmitCameraData(Camera * camera);
void SubmitLightingData();

struct DataRequiredForGameThread
{
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
};

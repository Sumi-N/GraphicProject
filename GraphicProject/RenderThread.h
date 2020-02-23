#pragma once
#include "Object.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "ConstantBuffer.h"

bool WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait);

void SignalTheDataHasBeenSubmitted();

void SubmitObjectData(Object * obj);
void SubmitCameraData(Camera * camera);
void SubmitLightingData();

struct DataRenderToGame
{
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
};

struct DataGameToRender
{
	ConstantData::Camera frame;
	ConstantData::Light light;
	std::vector<ConstantData::Object> drawcalllist;
	std::vector<ConstantData::Material> materiallist;
	std::vector<Object *> objectlist;
};

class RenderThread
{
public:
	void Init();
	void Run();
};
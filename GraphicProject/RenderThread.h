#pragma once
#include "Object.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "ConstantBuffer.h"

struct DataGameToRender
{
	ConstantData::Camera frame;
	ConstantData::Light light;
	std::vector<ConstantData::Mesh> const_mesh;
	std::vector<ConstantData::Material> const_material;
	std::vector<Object *> objectlist;
};

class RenderThread
{
public:
	void Init();
	void Run();

	bool WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait);

void SignalTheDataHasBeenSubmitted();

void SubmitObjectData(Object * obj);
void SubmitCameraData(Camera * camera);
void SubmitLightingData();
};
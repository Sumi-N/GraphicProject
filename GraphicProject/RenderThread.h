#pragma once
#include "Object.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "ConstantBuffer.h"

struct DataGameToRender
{
	ConstantData::Camera const_frame;
	std::vector<ConstantData::Model> const_model;
	std::vector<ConstantData::Material> const_material;
	ConstantData::Light const_light;
	std::vector<Object *> objectlist;
};

class RenderThread
{
public:
	void Init();
	void Run();

	void SubmitObjectData(Object * obj);
	void SubmitCameraData(Camera * camera);
	void SubmitLightingData();
};
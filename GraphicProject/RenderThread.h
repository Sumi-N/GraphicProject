#pragma once
#include "Object.h"
#include "Camera.h"
#include "CubeMap.h"
#include "FrameBuffer.h"
#include "ConstantBuffer.h"

struct DataGameToRender
{
	//Constant data that only need one
	ConstantData::Camera const_camera;
	ConstantData::Light const_light;
	ConstantData::Image const_image;

	// Object data
	std::vector<Object *> objectlist;
	std::vector<ConstantData::Model> const_model;
	std::vector<ConstantData::Material> const_material;

	// Image data
	std::vector<Object *> imagelist;
	std::vector<ConstantData::Model> const_image_model;
};

class RenderThread
{
public:
	void Init();
	void Run();

	void SubmitObjectData(Object * obj);
	void SubmitImageData(Object *obj);
	void SubmitCubeMapData(CubeMap * cubemap);
	void SubmitCameraData(Camera * camera);
	void SubmitLightingData();
};
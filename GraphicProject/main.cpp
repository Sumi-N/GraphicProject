#pragma once

#include "main.h"
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constatnt.h"
#include "Utility.h"
#include "Camera.h"
#include "Light.h"
#include "Quad.h"
#include "CubeMap.h"
#include "Event.h"
#include "ConstantBuffer.h"
#include "FrameBuffer.h"

// About threading
#include <thread>
#include "GameThread.h"
#include "Input.h"

//////////////////////Global Data
GLFWwindow * window;
Camera camera;
Object teapot;
AmbientLight ambientlight;
PointLight pointlight;
GLuint program;

Quad quad;
CubeMap cubemap;

struct DataRequiredForBuffer
{
	ConstantData::Camera frame;
	ConstantData::Light light;
	std::vector<ConstantData::Object> drawcalllist;
	std::vector<ConstantData::Material> materiallist;
	std::vector<Object *> objectlist;
};

ConstantBuffer const_buffer_frame;
ConstantBuffer const_buffer_drawcall;
ConstantBuffer const_buffer_material;
ConstantBuffer const_buffer_light;

FrameBuffer framebuffer;

DataRequiredForBuffer dataRequiredForBuffer[2];
DataRequiredForBuffer * BeginSubmittedByGameThread = &dataRequiredForBuffer[0];
DataRequiredForBuffer * BeginRenderedByRenderThread = &dataRequiredForBuffer[1];

DataRequiredForGameThread dataRequiredForGameThread[2];
DataRequiredForGameThread * BeginSubmittedByRenderThread = &dataRequiredForGameThread[0];
DataRequiredForGameThread * BeginReadByGameThread = &dataRequiredForGameThread[1];

Event FinishSubmittingAllDataFromGameThread;
Event CanSubmitDataFromApplicationThread;

bool WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait)
{
	return WaitForEvent(CanSubmitDataFromApplicationThread, i_timetowait);
}

void SignalTheDataHasBeenSubmitted()
{
	FinishSubmittingAllDataFromGameThread.Signal();
}

void SubmitObjectData(Object * obj)
{
	BeginSubmittedByGameThread->objectlist.push_back(obj);

	ConstantData::Object drawcall;
	drawcall.model_inverse_transpose_matrix = obj->mesh->model_vec_mat;
	drawcall.model_position_matrix = obj->mesh->model_pos_mat;
	BeginSubmittedByGameThread->drawcalllist.push_back(drawcall);

	ConstantData::Material material;
	material.specular = glm::vec4(obj->mesh->material->Ks[0], obj->mesh->material->Ks[1], obj->mesh->material->Ks[2], obj->mesh->material->Ns);
	material.diffuse = glm::vec4(obj->mesh->material->Kd[0], obj->mesh->material->Kd[1], obj->mesh->material->Kd[2], 1.0);
	BeginSubmittedByGameThread->materiallist.push_back(material);
}

void SubmitCameraData(Camera * camera)
{
	BeginSubmittedByGameThread->frame.camera_position_vector = camera->pos;
	BeginSubmittedByGameThread->frame.view_perspective_matrix = camera->view_perspective_mat;
}

void SubmitLightingData()
{
	BeginSubmittedByGameThread->light.light_ambient_intensity = glm::vec4(ambientlight.intensity, 1.0);
	BeginSubmittedByGameThread->light.light_point_intensity = glm::vec4(pointlight.intensity, 1.0);
	BeginSubmittedByGameThread->light.pointposition = glm::vec4(pointlight.position, 1.0);
}

void InitializeObject()
{
	Mesh* mesh = new Mesh();
	mesh->Load("../Assets/Meshes/teapot.obj");
	mesh->Init();

	Material* material = new Material();
	material->LoadShader("../Assets/Shaders/blinn_phong.vert.glsl", "../Assets/Shaders/blinn_phong.frag.glsl");

	teapot.SetMesh(mesh);
	mesh->SetMaterial(material);

	teapot.mesh->material->LoadTexture("../Assets/Textures/brick.png");
	teapot.mesh->material->LoadTexture("../Assets/Textures/brick-specular.png");

	// Setting up quad;
	quad.Initialize();
	quad.pos = glm::vec3(0, 0, -20);
	quad.scale = glm::vec3(1.0, 1.0, 1.0);
	quad.rot = glm::vec3(0, 0, 00);

	// Setting up position 
	teapot.pos = glm::vec3(0, 0, -50);
	teapot.scale = glm::vec3(1.0, 1.0, 1.0);
	teapot.rot = glm::vec3(-90, 0, 0);

	// Setup Light
	ambientlight.intensity = glm::vec3(0.1, 0.1, 0.1);
	pointlight.intensity = glm::vec3(1.0, 1.0, 1.0);
	pointlight.position = glm::vec3(20, 20, -50);

	// Setting up environment map
	cubemap.Initialize();
}

int InitializeRenderThread()
{
	// Initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		// If glfw initialization failed
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	// Register a function which will execute when the program finish 
	atexit(glfwTerminate);

	//Select OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "Interactive Graphics", NULL, NULL);
	if (window == NULL)
	{
		// If the window is not created
		std::cerr << "Can't create GLFW window." << std::endl;
		glfwTerminate();
		return 1;
	}
	// Attached the OpenGL to this window
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Set up callback
	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::cursorPositionCallback);

	// Set up culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Init Uniform buffer
	const_buffer_frame.Init(ConstantData::Index::Camera, ConstantData::Size::Camera);
	const_buffer_drawcall.Init(ConstantData::Index::Object, ConstantData::Size::Object);
	const_buffer_material.Init(ConstantData::Index::Material, ConstantData::Size::Material);
	const_buffer_light.Init(ConstantData::Index::Light, ConstantData::Size::Light);

	// Instantiate framebuffer
	framebuffer.Initialize(WIDTH, HEIGHT);
}

int main()
{
	InitializeRenderThread();

	InitializeObject();

	// Setup thread
	FinishSubmittingAllDataFromGameThread.Initialize(EventType::ResetAutomatically);
	CanSubmitDataFromApplicationThread.Initialize(EventType::ResetAutomatically, EventState::Signaled);
	std::thread gamethread(Application::Init);


	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		bool result = WaitForEvent(FinishSubmittingAllDataFromGameThread, 100000);
		if (result)
		{
			std::swap(BeginSubmittedByGameThread, BeginRenderedByRenderThread);
			std::swap(BeginReadByGameThread, BeginSubmittedByRenderThread);
			bool result = CanSubmitDataFromApplicationThread.Signal();
		}

		BeginSubmittedByRenderThread->up = false;
		BeginSubmittedByRenderThread->down = false;
		BeginSubmittedByRenderThread->right = false;
		BeginSubmittedByRenderThread->left = false;

		glfwPollEvents();

		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Cube mapping
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto & const_data_frame = BeginRenderedByRenderThread->frame;
		const_buffer_frame.Update(&const_data_frame);

		glDepthMask(GL_FALSE);
		glUseProgram(cubemap.mesh->material->programid);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.textureid);
		GLint vp_location = glGetUniformLocation(cubemap.mesh->material->programid, "view_perspective_matrix");
		glm::mat4 pos = const_data_frame.view_perspective_matrix * glm::translate(glm::mat4(1.0), const_data_frame.camera_position_vector);;
		glUniformMatrix4fv(vp_location, 1, GL_FALSE, &pos[0][0]);
		glUniform1i(cubemap.tmptexture.uniformid, 5);
		cubemap.mesh->Draw();
		glDepthMask(GL_TRUE);

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.bufferid);
		{
			// Renderring part
			{

				// Submit Camera Information
				auto & const_data_frame = BeginRenderedByRenderThread->frame;
				const_buffer_frame.Update(&const_data_frame);

				// Submit Light Information
				auto & const_data_light = BeginRenderedByRenderThread->light;
				const_buffer_light.Update(&const_data_light);

				for (int i = 0; i < BeginRenderedByRenderThread->objectlist.size(); i++)
				{
					auto & const_data_draw = BeginRenderedByRenderThread->drawcalllist[i];
					const_data_draw.model_view_perspective_matrix = const_data_frame.view_perspective_matrix * const_data_draw.model_position_matrix;
					const_buffer_drawcall.Update(&const_data_draw);

					auto & const_data_material = BeginRenderedByRenderThread->materiallist[i];
					const_buffer_material.Update(&const_data_material);

					// Submit shader program and get texture uniform
					BeginRenderedByRenderThread->objectlist[i]->mesh->material->BindShader();
					BeginRenderedByRenderThread->objectlist[i]->mesh->Draw();
				}
			}
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//{
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	// Submit Camera Information
		//	auto & const_data_frame = BeginRenderedByRenderThread->frame;
		//	const_buffer_frame.Update(&const_data_frame);

		//	for (int i = 0; i < BeginRenderedByRenderThread->objectlist.size(); i++)
		//	{
		//		auto & const_data_draw = BeginRenderedByRenderThread->drawcalllist[i];
		//		const_data_draw.mvp = const_data_frame.cvp * const_data_draw.mwt;
		//		const_buffer_drawcall.Update(&const_data_draw);

		//		glUseProgram(quad.mesh->material->programid);
		//		glActiveTexture(GL_TEXTURE0);
		//		glBindTexture(GL_TEXTURE_2D, framebuffer.targettexture);
		//		glUniform1i(glGetUniformLocation(quad.mesh->material->programid, "texture0"), 0);

		//		quad.mesh->Draw();
		//	}
		//}

		glfwSwapBuffers(window);

		// Clean up
		BeginRenderedByRenderThread->objectlist.clear();
		BeginRenderedByRenderThread->drawcalllist.clear();
		BeginRenderedByRenderThread->materiallist.clear();
	}
}
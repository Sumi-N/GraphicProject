#pragma once

#include "main.h"
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constatnt.h"
#include "Utility.h"
#include "FileLoader.h"
#include "Camera.h"
#include "Light.h"
#include "Quad.h"
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

struct DataRequiredForBuffer
{
	ConstantBufferFormat::Frame frame;
	ConstantBufferFormat::Light light;
	std::vector<ConstantBufferFormat::DrawCall> drawcalllist;
	std::vector<ConstantBufferFormat::Material> materiallist;
	std::vector<Object *> objectlist;
};

ConstantBuffer const_buffer_frame(ConstantBufferTypes::Frame);
ConstantBuffer const_buffer_drawcall(ConstantBufferTypes::DrawCall);
ConstantBuffer const_buffer_material(ConstantBufferTypes::Material);
ConstantBuffer const_buffer_light(ConstantBufferTypes::Light);

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

	ConstantBufferFormat::DrawCall drawcall;
	drawcall.mit = obj->mesh->model_vec_mat;
	drawcall.mwt = obj->mesh->model_pos_mat;
	BeginSubmittedByGameThread->drawcalllist.push_back(drawcall);

	ConstantBufferFormat::Material material;
	material.specular = glm::vec4(obj->mesh->material->Ks[0], obj->mesh->material->Ks[1], obj->mesh->material->Ks[2], obj->mesh->material->Ns);
	material.diffuse = glm::vec4(obj->mesh->material->Kd[0], obj->mesh->material->Kd[1], obj->mesh->material->Kd[2], 1.0);
	BeginSubmittedByGameThread->materiallist.push_back(material);
}

void SubmitCameraData(Camera * camera)
{
	BeginSubmittedByGameThread->frame.cwp = camera->pos;
	BeginSubmittedByGameThread->frame.cvp = camera->view_perspective_mat;
}

void SubmitLightingData()
{
	BeginSubmittedByGameThread->light.ambientintensity = glm::vec4(ambientlight.intensity, 1.0);
	BeginSubmittedByGameThread->light.pointintensity = glm::vec4(pointlight.intensity, 1.0);
	BeginSubmittedByGameThread->light.pointposition = glm::vec4(pointlight.position, 1.0);
}

void InitializeObject()
{
	Mesh* mesh = new Mesh();
	mesh->Load("../Objfiles/teapot.obj");
	mesh->Init();

	Material* material = new Material();
	material->Load("blinn_phong.vert.glsl", "blinn_phong.frag.glsl");

	teapot.SetMesh(mesh);
	mesh->SetMaterial(material);

	teapot.mesh->material->LoadTexture("../Objfiles/brick.png");
	teapot.mesh->material->LoadTexture("../Objfiles/brick-specular.png");

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

	// Bind Uniform buffer
	const_buffer_drawcall.Bind();
	const_buffer_frame.Bind();
	const_buffer_material.Bind();
	const_buffer_light.Bind();

	// Instantiate framebuffer
	framebuffer.Initialize(WIDTH, HEIGHT);

	GLuint textureid;
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

	Texture tmptexture;
	int tmpwidth, tmpheight;

	std::vector<cy::Color24> data;
	tmptexture.Load("../Objfiles/cubemap_posx.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	std::vector<cy::Color24> data2;
	tmptexture.Load("../Objfiles/cubemap_negx.png", data2, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data2.data());

	std::vector<cy::Color24> data3;
	tmptexture.Load("../Objfiles/cubemap_posy.png", data3, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data3.data());

	std::vector<cy::Color24> data4;
	tmptexture.Load("../Objfiles/cubemap_negy.png", data4, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data4.data());

	std::vector<cy::Color24> data5;
	tmptexture.Load("../Objfiles/cubemap_posz.png", data5, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data5.data());

	std::vector<cy::Color24> data6;
	tmptexture.Load("../Objfiles/cubemap_negz.png", data6, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data6.data());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.bufferid);
		{
			// Renderring part
			{
				// Clear window
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Submit Camera Information
				auto & const_data_frame = BeginRenderedByRenderThread->frame;
				const_buffer_frame.Update(&const_data_frame);

				// Submit Light Information
				auto & const_data_light = BeginRenderedByRenderThread->light;
				const_buffer_light.Update(&const_data_light);

				for (int i = 0; i < BeginRenderedByRenderThread->objectlist.size(); i++)
				{
					auto & const_data_draw = BeginRenderedByRenderThread->drawcalllist[i];
					const_data_draw.mvp = const_data_frame.cvp * const_data_draw.mwt;
					const_buffer_drawcall.Update(&const_data_draw);

					auto & const_data_material = BeginRenderedByRenderThread->materiallist[i];
					const_buffer_material.Update(&const_data_material);

					// Submit shader program and get texture unifrom
					BeginRenderedByRenderThread->objectlist[i]->mesh->material->BindShader();
					BeginRenderedByRenderThread->objectlist[i]->mesh->Draw();
				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Submit Camera Information
			auto & const_data_frame = BeginRenderedByRenderThread->frame;
			const_buffer_frame.Update(&const_data_frame);

			for (int i = 0; i < BeginRenderedByRenderThread->objectlist.size(); i++)
			{
				auto & const_data_draw = BeginRenderedByRenderThread->drawcalllist[i];
				const_data_draw.mvp = const_data_frame.cvp * const_data_draw.mwt;
				const_buffer_drawcall.Update(&const_data_draw);

				glUseProgram(quad.mesh->material->programid);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, framebuffer.targettexture);
				glUniform1i(glGetUniformLocation(quad.mesh->material->programid, "texture0"), 0);

				quad.mesh->Draw();
			}
		}

		glfwSwapBuffers(window);

		// Clean up
		BeginRenderedByRenderThread->objectlist.clear();
		BeginRenderedByRenderThread->drawcalllist.clear();
		BeginRenderedByRenderThread->materiallist.clear();
	}
}
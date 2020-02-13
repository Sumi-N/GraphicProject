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
#include "Event.h"
#include "ConstantBuffer.h"

struct DataRequiredForBuffer
{
	ConstantBufferFormat::Frame frame;
	std::vector<ConstantBufferFormat::DrawCall> drawcalllist;
	std::vector<Object *> objectlist;
};

ConstantBuffer const_buffer_frame(ConstantBufferTypes::Frame);
ConstantBuffer const_buffer_drawcall(ConstantBufferTypes::DrawCall);

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
}

void SubmitCameraData(Camera * camera)
{
	BeginSubmittedByGameThread->frame.cwp = camera->pos;
	BeginSubmittedByGameThread->frame.cvp = camera->view_perspective_mat;
}

// About threading
#include <thread>
#include <mutex>
#include "GameThread.h"
#include "Input.h"

//////////////////////Global Data
GLFWwindow * window;
Camera camera;
Object teapot;
AmbientLight ambientlight;
PointLight pointlight;
GLuint program;

void InitializeObject()
{
	Mesh* mesh = new Mesh();
	mesh->Load("../Objfiles/teapot.obj");
	mesh->Init();

	Material* material = new Material();
	material->Load("point.vert.glsl", "point.frag.glsl");

	teapot.SetMesh(mesh);
	mesh->SetMaterial(material);

	teapot.mesh->texture = new Texture();
	teapot.mesh->texture->Load("../Objfiles/brick.png");

	// Setting up position 
	teapot.pos = glm::vec3(0, 0, -50);
	teapot.scale = glm::vec3(1.0, 1.0, 1.0);
	teapot.rot = glm::vec3(-90, 0, 0);
}

int Initialize()
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

	FinishSubmittingAllDataFromGameThread.Initialize(EventType::ResetAutomatically);
	CanSubmitDataFromApplicationThread.Initialize(EventType::ResetAutomatically, EventState::Signaled);

	// Bind Uniform buffer
	const_buffer_drawcall.Bind();
	const_buffer_frame.Bind();

	InitializeObject();
}

int main()
{
	Initialize();

	std::thread gamethread(Application::Init);

	// Setup Light
	ambientlight.intensity = glm::vec3(0.1, 0.1, 0.1);
	pointlight.intensity = glm::vec3(1.0, 1.0, 1.0);
	pointlight.position = glm::vec3(20, 20, -50);

	Texture * pottexturespecular = new Texture;
	pottexturespecular->Load("../Objfiles/brick-specular.png");

	GLuint TextureObj;
	glGenTextures(1, &TextureObj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, teapot.mesh->texture->width, teapot.mesh->texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, teapot.mesh->texture->data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureObj);

	GLuint TextureObj2;
	glGenTextures(1, &TextureObj2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pottexturespecular->width, pottexturespecular->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pottexturespecular->data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureObj);

	program = teapot.mesh->material->programid;

	//GLint mvplocation = glGetUniformLocation(program, "mvp");
	//if (mvplocation == -1)
	//{
	//	std::cerr << "The mvplocation variable doesn't exist in the shader file" << std::endl;
	//}

	//GLint modelmatrixlocation = glGetUniformLocation(program, "modelmatrix");
	//if (modelmatrixlocation == -1)
	//{
	//	std::cerr << "The modelmatrix variable doesn't exist in the shader file" << std::endl;
	//}

	//GLint cameraposition = glGetUniformLocation(program, "cameraposition");
	//if (cameraposition == -1)
	//{
	//	std::cerr << "The cameraposition variable doesn't exist in the shader file" << std::endl;
	//}

	//GLint mtransposelocation = glGetUniformLocation(program, "mtranspose");
	//if (mtransposelocation == -1)
	//{
	//	std::cerr << "The mtransposelocation variable doesn't exist in the shader file" << std::endl;
	//}

	GLint ambientintensity = glGetUniformLocation(program, "ambientintensity");
	if (ambientintensity == -1)
	{
		std::cerr << "The ambientintensity variable doesn't exist in the shader file" << std::endl;
	}

	GLint diffuselocation = glGetUniformLocation(program, "diffuse");
	if (diffuselocation == -1)
	{
		std::cerr << "The diffuselocation variable doesn't exist in the shader file" << std::endl;
	}

	GLint specularlocation = glGetUniformLocation(program, "specular");
	if (specularlocation == -1)
	{
		std::cerr << "The specularlocation variable doesn't exist in the shader file" << std::endl;
	}

	GLint pointintensitylocation = glGetUniformLocation(program, "pointintensity");
	if (pointintensitylocation == -1)
	{
		std::cerr << "The pointintensitylocation variable doesn't exist in the shader file" << std::endl;
	}

	GLint pointpositionlocation = glGetUniformLocation(program, "pointposition");
	if (pointpositionlocation == -1)
	{
		std::cerr << "The pointpositionlocation variable doesn't exist in the shader file" << std::endl;
	}

	GLint gSampler = glGetUniformLocation(program, "gSampler");
	if (gSampler == -1)
	{
		std::cerr << "The gSampler variable doesn't exist in the shader file" << std::endl;
	}

	GLint gSampler2 = glGetUniformLocation(program, "gSampler2");
	if (gSampler2 == -1)
	{
		std::cerr << "The gSampler2 variable doesn't exist in the shader file" << std::endl;
	}

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

		// clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		

		{
			auto & const_data_frame = BeginRenderedByRenderThread->frame;
			const_buffer_frame.Update(&const_data_frame);

			glUniform3f(diffuselocation, teapot.mesh->material->Kd[0], teapot.mesh->material->Kd[1], teapot.mesh->material->Kd[2]);
			glUniform4f(specularlocation, teapot.mesh->material->Ks[0], teapot.mesh->material->Ks[1], teapot.mesh->material->Ks[2], teapot.mesh->material->Ns);
			glUniform1i(gSampler, 0);
			glUniform1i(gSampler2, 1);

			// Lighting data
			glUniform3f(ambientintensity, ambientlight.intensity.x, ambientlight.intensity.y, ambientlight.intensity.z);
			glUniform3f(pointintensitylocation, pointlight.intensity.r, pointlight.intensity.g, pointlight.intensity.b);
			glUniform3f(pointpositionlocation, pointlight.position.x, pointlight.position.y, pointlight.position.z);

			for (int i = 0; i < BeginRenderedByRenderThread->objectlist.size(); i++)
			{
				auto & const_data_draw = BeginRenderedByRenderThread->drawcalllist[i];
				const_data_draw.mvp =  const_data_frame.cvp * const_data_draw.mwt;
				const_buffer_drawcall.Update(&const_data_draw);
				BeginRenderedByRenderThread->objectlist[i]->mesh->material->BindShader();
				BeginRenderedByRenderThread->objectlist[i]->mesh->Draw();
			}
			

			glfwSwapBuffers(window);

			BeginRenderedByRenderThread->objectlist.clear();
			BeginRenderedByRenderThread->drawcalllist.clear();
		}
	}
}
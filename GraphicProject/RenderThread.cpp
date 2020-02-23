#pragma once
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

// About threading
#include <thread>
#include "RenderThread.h"
#include "GameThread.h"
#include "Input.h"

//////////////////////Global Data
extern GameThread gamethread;
extern std::mutex mutex;
extern std::condition_variable condition_variable;

GLFWwindow * window;

extern Camera camera;
extern Object teapot;
extern AmbientLight ambientlight;
extern PointLight pointlight;
extern Quad quad;
extern CubeMap cubemap;

ConstantBuffer buffer_camera;
ConstantBuffer buffer_object;
ConstantBuffer buffer_material;
ConstantBuffer buffer_light;
FrameBuffer framebuffer;

DataGameToRender datagametorender[2];
DataGameToRender * datagameown = &datagametorender[0];
DataGameToRender * datarenderown = &datagametorender[1];

extern Event event_done_submitting_from_game;
extern Event event_can_submit_from_game;

bool RenderThread::WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait)
{
	return WaitForEvent(event_can_submit_from_game, i_timetowait);
}

void RenderThread::SignalTheDataHasBeenSubmitted()
{
	event_done_submitting_from_game.Signal();
}

void RenderThread::Init()
{
	// Initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		// If glfw initialization failed
		std::cerr << "Can't initialize GLFW" << std::endl;
		return;
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
		return;
	}
	// Attached the OpenGL to this window
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Can't initialize GLEW" << std::endl;
		return;
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
	buffer_camera.Init(ConstantData::Index::Camera, ConstantData::Size::Camera);
	buffer_object.Init(ConstantData::Index::Object, ConstantData::Size::Object);
	buffer_material.Init(ConstantData::Index::Material, ConstantData::Size::Material);
	buffer_light.Init(ConstantData::Index::Light, ConstantData::Size::Light);

	// Instantiate framebuffer
	framebuffer.Init(WIDTH, HEIGHT);


	////////////////////// Separate the content below later ///////////////////////

			// Initialize objects
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
	quad.Init();
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
	pointlight.position = glm::vec3(0, 0, -20);

	// Setting up environment map
	cubemap.Init();
}

void RenderThread::Run()
{
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		bool result = WaitForEvent(event_done_submitting_from_game, 100000);
		if (result)
		{
			std::swap(datagameown, datarenderown);
			gamethread.RenderToGameInfo();
			
			bool result = event_can_submit_from_game.Signal();
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
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				// Submit Camera Information
				auto & const_data_frame = datarenderown->frame;
				buffer_camera.Update(&const_data_frame);

				// Submit Light Information
				auto & const_data_light = datarenderown->light;
				buffer_light.Update(&const_data_light);

				for (int i = 0; i < datarenderown->objectlist.size(); i++)
				{
					auto & const_data_draw = datarenderown->const_mesh[i];
					const_data_draw.model_view_perspective_matrix = const_data_frame.view_perspective_matrix * const_data_draw.model_position_matrix;
					buffer_object.Update(&const_data_draw);

					auto & const_data_material = datarenderown->const_material[i];
					buffer_material.Update(&const_data_material);

					// Submit shader program and get texture uniform
					datarenderown->objectlist[i]->mesh->material->BindShader();
					datarenderown->objectlist[i]->mesh->Draw();
				}
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			auto & const_data_frame = datarenderown->frame;
			buffer_camera.Update(&const_data_frame);

			glDepthMask(GL_FALSE);
			cubemap.mesh->material->BindShader();
			GLint vp_location = glGetUniformLocation(cubemap.mesh->material->programid, "view_perspective_matrix");
			glm::mat4 pos = const_data_frame.view_perspective_matrix * glm::translate(glm::mat4(1.0), const_data_frame.camera_position_vector);;
			glUniformMatrix4fv(vp_location, 1, GL_FALSE, &pos[0][0]);
			cubemap.mesh->Draw();
			glDepthMask(GL_TRUE);
		}


		{
			// Submit Camera Information
			auto & const_data_frame = datarenderown->frame;
			buffer_camera.Update(&const_data_frame);

			for (int i = 0; i < datarenderown->objectlist.size(); i++)
			{
				auto & const_data_draw = datarenderown->const_mesh[i];
				const_data_draw.model_view_perspective_matrix = const_data_frame.view_perspective_matrix * const_data_draw.model_position_matrix;
				buffer_object.Update(&const_data_draw);

				quad.Bind(framebuffer);
				quad.mesh->Draw();
			}
		}

		glfwSwapBuffers(window);

		// Clean up
		datarenderown->objectlist.clear();
		datarenderown->const_mesh.clear();
		datarenderown->const_material.clear();
	}
}


void RenderThread::SubmitObjectData(Object * obj)
{
	datagameown->objectlist.push_back(obj);

	ConstantData::Mesh mesh;
	mesh.model_inverse_transpose_matrix = obj->mesh->model_inverse_transpose_matrix;
	mesh.model_position_matrix = obj->mesh->model_pos_mat;
	datagameown->const_mesh.push_back(mesh);

	ConstantData::Material material;
	material.specular = glm::vec4(obj->mesh->material->Ks[0], obj->mesh->material->Ks[1], obj->mesh->material->Ks[2], obj->mesh->material->Ns);
	material.diffuse = glm::vec4(obj->mesh->material->Kd[0], obj->mesh->material->Kd[1], obj->mesh->material->Kd[2], 1.0);
	datagameown->const_material.push_back(material);
}

void RenderThread::SubmitCameraData(Camera * camera)
{
	datagameown->frame.camera_position_vector = camera->pos;
	datagameown->frame.view_perspective_matrix = camera->view_perspective_mat;
}

void RenderThread::SubmitLightingData()
{
	datagameown->light.light_ambient_intensity = glm::vec4(ambientlight.intensity, 1.0);
	datagameown->light.light_point_intensity = glm::vec4(pointlight.intensity, 1.0);
	datagameown->light.pointposition = glm::vec4(pointlight.position, 0.0);
}
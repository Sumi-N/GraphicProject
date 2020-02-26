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
#include "ConstantBuffer.h"

// About threading
#include <thread>
#include "RenderThread.h"
#include "GameThread.h"
#include "Input.h"

//////////////////////Global Data
extern GameThread gamethread;
extern std::mutex mutex_render;
extern std::mutex mutex_game;
extern std::condition_variable cv_render;
extern std::condition_variable cv_game;
extern bool brenderready;
extern bool bgameready;

GLFWwindow * window;

DataGameToRender datagametorender[2];
DataGameToRender * datagameown = &datagametorender[0];
DataGameToRender * datarenderown = &datagametorender[1];

extern Camera camera;
extern Object teapot;
extern AmbientLight ambientlight;
extern PointLight pointlight;
extern Quad quad;
extern CubeMap cubemap;

ConstantBuffer buffer_camera;
ConstantBuffer buffer_mesh;
ConstantBuffer buffer_material;
ConstantBuffer buffer_light;
ConstantBuffer buffer_image;
FrameBuffer framebuffer;

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
	glEnable(GL_STENCIL_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Init Uniform buffer
	buffer_camera.Init(ConstantData::Index::Camera, ConstantData::Size::Camera);
	buffer_mesh.Init(ConstantData::Index::Object, ConstantData::Size::Object);
	buffer_material.Init(ConstantData::Index::Material, ConstantData::Size::Material);
	buffer_light.Init(ConstantData::Index::Light, ConstantData::Size::Light);
	buffer_image.Init(ConstantData::Index::Image, ConstantData::Size::Image);

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
	quad.pos = glm::vec3(0, -7, -50);
	quad.scale = glm::vec3(15.0, 15.0, 15.0);
	quad.rot = glm::vec3(-90, 0, 00);

	// Setting up position 
	teapot.pos = glm::vec3(0, -5, -50);
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
		glfwPollEvents();

		// Critiacal Section
		{
			std::lock_guard<std::mutex> lock_guard(mutex_render);
			
			{
				std::swap(datagameown, datarenderown);
				gamethread.RenderToGameInfo();
			}

			brenderready = true;
			cv_render.notify_one();
		}

		Input::ClearInput();
		{
			// Update uniform data common for frame
			// Submit Camera Information
			auto & const_data_camera = datarenderown->const_camera;
			buffer_camera.Update(&const_data_camera);
			// Submit Light Information
			auto & const_data_light = datarenderown->const_light;
			buffer_light.Update(&const_data_light);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.bufferid);
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// Rendering Background
				{
					glDepthMask(GL_FALSE);
					cubemap.vp = const_data_camera.perspective_matrix * glm::mat4(glm::mat3(const_data_camera.view_matrix)) * glm::mat4(glm::mat3(datarenderown->const_image.mirror_matrix));
					cubemap.Bind();
					cubemap.mesh->Draw();
					glDepthMask(GL_TRUE);
				}

				for (int i = 0; i < datarenderown->objectlist.size(); i++)
				{
					auto & const_data_draw = datarenderown->const_model[i];

					// Instead of getting mvp, gets mirror image
					const_data_draw.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * datarenderown->const_image.mirror_matrix * const_data_draw.model_position_matrix;
					buffer_mesh.Update(&const_data_draw);

					auto & const_data_material = datarenderown->const_material[i];
					buffer_material.Update(&const_data_material);

					datarenderown->objectlist[i]->mesh->material->BindSkyBox(cubemap);
					datarenderown->objectlist[i]->mesh->material->BindShader();
					datarenderown->objectlist[i]->mesh->Draw();
				}
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				// Rendering Background
				{
					glDepthMask(GL_FALSE);
					cubemap.vp = const_data_camera.perspective_matrix * glm::mat4(glm::mat3(const_data_camera.view_matrix));
					cubemap.Bind();
					cubemap.mesh->Draw();
					glDepthMask(GL_TRUE);
				}

				
				// Rendering Objects
				for (int i = 0; i < datarenderown->objectlist.size(); i++)
				{
					auto & const_data_draw = datarenderown->const_model[i];
					const_data_draw.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * const_data_draw.model_position_matrix;
					buffer_mesh.Update(&const_data_draw);

					auto & const_data_material = datarenderown->const_material[i];
					buffer_material.Update(&const_data_material);

					datarenderown->objectlist[i]->mesh->material->BindSkyBox(cubemap);
					datarenderown->objectlist[i]->mesh->material->BindShader();
					datarenderown->objectlist[i]->mesh->Draw();
				}

				// Render Mirror Image
				{
					for (int i = 0; i < datarenderown->imagelist.size(); i++)
					{
						auto & const_data_model = datarenderown->const_image_model[i];
						const_data_model.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * const_data_model.model_position_matrix;
						buffer_mesh.Update(&const_data_model);

						// This part need to change later
						quad.Bind(framebuffer);
						datarenderown->imagelist[i]->mesh->Draw();
					}
				}
			}
		}

		glfwSwapBuffers(window);

		// Clean up
		datarenderown->objectlist.clear();
		datarenderown->const_model.clear();
		datarenderown->const_material.clear();
		datarenderown->imagelist.clear();
		datarenderown->const_image_model.clear();
	}
}

void RenderThread::SubmitObjectData(Object * obj)
{
	datagameown->objectlist.push_back(obj);

	ConstantData::Model model;
	model.model_inverse_transpose_matrix = obj->mesh->model_inverse_transpose_matrix;
	model.model_position_matrix = obj->mesh->model_pos_mat;
	datagameown->const_model.push_back(model);

	ConstantData::Material material;
	material.specular = glm::vec4(obj->mesh->material->Ks[0], obj->mesh->material->Ks[1], obj->mesh->material->Ks[2], obj->mesh->material->Ns);
	material.diffuse = glm::vec4(obj->mesh->material->Kd[0], obj->mesh->material->Kd[1], obj->mesh->material->Kd[2], 1.0);
	datagameown->const_material.push_back(material);
}

void RenderThread::SubmitImageData(Object * image)
{
	datagameown->imagelist.push_back(image);

	ConstantData::Model model;
	model.model_inverse_transpose_matrix = image->mesh->model_inverse_transpose_matrix;
	model.model_position_matrix = image->mesh->model_pos_mat;
	datagameown->const_image_model.push_back(model);

	Quad* quad = static_cast<Quad *>(image);
	datagameown->const_image.mirror_matrix = quad->mirror_inverse_mat;
}

void RenderThread::SubmitCubeMapData(CubeMap * cubemap)
{
}

void RenderThread::SubmitCameraData(Camera * camera)
{
	datagameown->const_camera.camera_position_vector = camera->pos;
	datagameown->const_camera.perspective_matrix = camera->perspective;
	datagameown->const_camera.view_matrix = camera->view;
}

void RenderThread::SubmitLightingData()
{
	datagameown->const_light.light_ambient_intensity = glm::vec4(ambientlight.intensity, 1.0);
	datagameown->const_light.light_point_intensity = glm::vec4(pointlight.intensity, 1.0);
	datagameown->const_light.pointposition = glm::vec4(pointlight.position, 0.0);
}
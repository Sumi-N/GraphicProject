#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

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

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.bufferid);
		//{
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//	// Rendering Background
		//	{
		//		glDepthMask(GL_FALSE);
		//		auto & const_data_frame = datarenderown->const_camera;
		//		buffer_camera.Update(&const_data_frame);
		//		cubemap.mesh->material->BindShader();
		//		GLint vp_location = glGetUniformLocation(cubemap.mesh->material->programid, "view_perspective_matrix");
		//		glm::mat4 pos = const_data_frame.perspective_matrix * glm::mat4(glm::mat3(const_data_frame.view_matrix)) * glm::mat4(glm::mat3(datarenderown->const_image.mirror_matrix));
		//		glUniformMatrix4fv(vp_location, 1, GL_FALSE, &pos[0][0]);
		//		cubemap.mesh->Draw();
		//		glDepthMask(GL_TRUE);
		//	}

		//	{
		//		// Submit Camera Information
		//		auto & const_data_camera = datarenderown->const_camera;
		//		buffer_camera.Update(&const_data_camera);

		//		// Submit Light Information
		//		auto & const_data_light = datarenderown->const_light;
		//		buffer_light.Update(&const_data_light);

		//		for (int i = 0; i < datarenderown->objectlist.size(); i++)
		//		{
		//			auto & const_data_draw = datarenderown->const_model[i];

		//			// Instead of getting mvp, gets mirror image
		//			const_data_draw.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * datarenderown->const_image.mirror_matrix * const_data_draw.model_position_matrix;
		//			buffer_mesh.Update(&const_data_draw);

		//			auto & const_data_material = datarenderown->const_material[i];
		//			buffer_material.Update(&const_data_material);

		//			///////////////////////////////////////////////
		//			// Submit shader program and get texture uniform
		//			datarenderown->objectlist[i]->mesh->material->BindShader();

		//			GLint uniformid = glGetUniformLocation(datarenderown->objectlist[i]->mesh->material->programid, "skybox");
		//			if (uniformid == -1)
		//			{
		//				//std::cerr << "The skybox2 variable doesn't exist in the shader file" << std::endl;
		//			}
		//			Texture * cubetexture = cubemap.GetCubeMapTexture();
		//			glActiveTexture(GL_TEXTURE0 + cubetexture->unitnumber);
		//			glBindTexture(GL_TEXTURE_2D, cubetexture->textureid);
		//			glUniform1i(uniformid, cubetexture->unitnumber);
		//			////////////////////////////////////////////////

		//			datarenderown->objectlist[i]->mesh->Draw();
		//		}
		//	}
		//}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// Rendering Background
			{
				glDepthMask(GL_FALSE);
				auto & const_data_frame = datarenderown->const_camera;
				buffer_camera.Update(&const_data_frame);
				cubemap.mesh->material->BindShader();
				GLint vp_location = glGetUniformLocation(cubemap.mesh->material->programid, "view_perspective_matrix");
				glm::mat4 pos = const_data_frame.perspective_matrix * glm::mat4(glm::mat3(const_data_frame.view_matrix));
				glUniformMatrix4fv(vp_location, 1, GL_FALSE, &pos[0][0]);
				cubemap.mesh->Draw();
				glDepthMask(GL_TRUE);
			}

			// Rendering Objects
			{
				// Submit Camera Information
				auto & const_data_camera = datarenderown->const_camera;
				buffer_camera.Update(&const_data_camera);

				// Submit Light Information
				auto & const_data_light = datarenderown->const_light;
				buffer_light.Update(&const_data_light);

				for (int i = 0; i < datarenderown->objectlist.size(); i++)
				{
					auto & const_data_draw = datarenderown->const_model[i];
					const_data_draw.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * const_data_draw.model_position_matrix;
					buffer_mesh.Update(&const_data_draw);

					auto & const_data_material = datarenderown->const_material[i];
					buffer_material.Update(&const_data_material);

					///////////////////////////////////////////////
					// Submit shader program and get texture uniform
					datarenderown->objectlist[i]->mesh->material->BindShader();

					GLint uniformid = glGetUniformLocation(datarenderown->objectlist[i]->mesh->material->programid, "skybox");
					if (uniformid == -1)
					{
						//std::cerr << "The skybox2 variable doesn't exist in the shader file" << std::endl;
					}
					Texture * cubetexture = cubemap.GetCubeMapTexture();
					glActiveTexture(GL_TEXTURE0 + cubetexture->unitnumber);
					glBindTexture(GL_TEXTURE_2D, cubetexture->textureid);

					glUniform1i(uniformid, cubetexture->unitnumber);
					////////////////////////////////////////////////

					datarenderown->objectlist[i]->mesh->Draw();
				}
			}

			{
				// Submit Camera Information
				auto & const_data_camera = datarenderown->const_camera;
				buffer_camera.Update(&const_data_camera);

				for (int i = 0; i < datarenderown->imagelist.size(); i++)
				{
					auto & const_data_model = datarenderown->const_image_model[i];
					const_data_model.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * const_data_model.model_position_matrix;
					buffer_mesh.Update(&const_data_model);

					// This part need to change later
					quad.Bind(framebuffer);

					glEnable(GL_STENCIL_TEST);
					glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
					glStencilMask(0xFF); // Write to stencil buffer
					glDepthMask(GL_FALSE);
					glClear(GL_STENCIL_BUFFER_BIT);
					quad.mesh->Draw();
					glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
					glStencilMask(0x00); // Don't write anything to stencil buffer
					glDepthMask(GL_TRUE); // Write to depth buffer
				}
			}

			// Rendering Background
			{
				glDepthMask(GL_FALSE);
				auto & const_data_frame = datarenderown->const_camera;
				buffer_camera.Update(&const_data_frame);
				cubemap.mesh->material->BindShader();
				GLint vp_location = glGetUniformLocation(cubemap.mesh->material->programid, "view_perspective_matrix");
				glm::mat4 pos = const_data_frame.perspective_matrix * glm::mat4(glm::mat3(const_data_frame.view_matrix)) * glm::mat4(glm::mat3(datarenderown->const_image.mirror_matrix));
				glUniformMatrix4fv(vp_location, 1, GL_FALSE, &pos[0][0]);
				cubemap.mesh->Draw();
				glDepthMask(GL_TRUE);
			}

			// Rendering Objects
			{
				// Submit Camera Information
				auto & const_data_camera = datarenderown->const_camera;
				buffer_camera.Update(&const_data_camera);

				// Submit Light Information
				auto & const_data_light = datarenderown->const_light;
				buffer_light.Update(&const_data_light);

				for (int i = 0; i < datarenderown->objectlist.size(); i++)
				{
					auto & const_data_draw = datarenderown->const_model[i];
					const_data_draw.model_view_perspective_matrix = const_data_camera.perspective_matrix * const_data_camera.view_matrix * datarenderown->const_image.mirror_matrix * const_data_draw.model_position_matrix;
					buffer_mesh.Update(&const_data_draw);

					auto & const_data_material = datarenderown->const_material[i];
					buffer_material.Update(&const_data_material);

					///////////////////////////////////////////////
					// Submit shader program and get texture uniform
					datarenderown->objectlist[i]->mesh->material->BindShader();

					GLint uniformid = glGetUniformLocation(datarenderown->objectlist[i]->mesh->material->programid, "skybox");
					if (uniformid == -1)
					{
						//std::cerr << "The skybox2 variable doesn't exist in the shader file" << std::endl;
					}
					Texture * cubetexture = cubemap.GetCubeMapTexture();
					glActiveTexture(GL_TEXTURE0 + cubetexture->unitnumber);
					glBindTexture(GL_TEXTURE_2D, cubetexture->textureid);

					glUniform1i(uniformid, cubetexture->unitnumber);
					////////////////////////////////////////////////

					datarenderown->objectlist[i]->mesh->Draw();
				}
			}
			glDisable(GL_STENCIL_TEST);
		}

		glfwSwapBuffers(window);

		// Clean up
		datarenderown->objectlist.clear();
		datarenderown->const_model.clear();
		datarenderown->const_material.clear();
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

	ConstantData::Material material;
	material.specular = glm::vec4(image->mesh->material->Ks[0], image->mesh->material->Ks[1], image->mesh->material->Ks[2], image->mesh->material->Ns);
	material.diffuse = glm::vec4(image->mesh->material->Kd[0], image->mesh->material->Kd[1], image->mesh->material->Kd[2], 1.0);
	datagameown->const_image_material.push_back(material);

	glm::vec3 p = glm::normalize(glm::mat3(image->mesh->model_pos_mat) * glm::vec3(0, 0, 1));
	float mirror_floats[16] =
	{
		1 - 2 * p.x * p.x, -2 * p.y * p.x, -2 * p.z * p.x, 0.0,
		-2 * p.x * p.y, 1 - 2 * p.y * p.y, -2 * p.z * p.y, 0.0,
		-2 * p.x * p.z, -2 * p.y * p.z, 1 - 2 * p.z * p.z, 0.0,
		-2 * p.x * 7, -2 * p.y * 7, -2 * p.z * 7, 1.0,
	};
	glm::mat4 mirror_mat = glm::make_mat4(mirror_floats);
	datagameown->const_image.mirror_matrix = mirror_mat;
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
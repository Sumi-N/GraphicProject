#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constatnt.h"
#include "Utility.h"
#include "FileLoader.h"
#include "Input.h"
#include "Camera.h"
#include "Light.h"

// About threading
#include <thread>
#include <mutex>
#include "GameThread.h"

std::mutex mtx;

GLFWwindow * window;
Camera camera;
Object teapot;
AmbientLight ambientlight;
PointLight pointlight;
glm::vec3 velocity;
//Timer timer;
//bool isReadyReadBuffer = false;

int main()
{
	//timer.Init();
	// Start the game thread
	std::thread gamethread(Application::Init);

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
	if (glewInit() != GLEW_OK) {
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	// Setup Light
	ambientlight.intensity = glm::vec3(0.1, 0.1, 0.1);
	pointlight.intensity = glm::vec3(1.0, 1.0, 1.0);
	pointlight.position = glm::vec3(20, 20, -50);

	Texture * pottexturespecular = new Texture;
	pottexturespecular->Load("../Objfiles/brick-specular.png");

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint IndexBuffer;
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.mesh->index.size() * sizeof(teapot.mesh->index[0]), teapot.mesh->index.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.mesh->data.size() * sizeof(teapot.mesh->data[0]), teapot.mesh->data.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(teapot.mesh->data[0]), (void*)(0));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(teapot.mesh->data[0]), (void*)(sizeof(cy::Point3f)));

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(teapot.mesh->data[0]), (void*)(2 * sizeof(cy::Point3f)));

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

	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint program = FileLoader::loadShaderProgram(VERTEXSHADERPATH, FRAGMENTSHADERPATH);

	GLint mvplocation = glGetUniformLocation(program, "mvp");
	if (mvplocation == -1)
	{
		std::cerr << "The mvplocation variable doesn't exist in the shader file" << std::endl;
	}

	GLint modelmatrixlocation = glGetUniformLocation(program, "modelmatrix");
	if (modelmatrixlocation == -1)
	{
		std::cerr << "The modelmatrix variable doesn't exist in the shader file" << std::endl;
	}

	GLint cameraposition = glGetUniformLocation(program, "cameraposition");
	if (cameraposition == -1)
	{
		std::cerr << "The cameraposition variable doesn't exist in the shader file" << std::endl;
	}

	GLint mtransposelocation = glGetUniformLocation(program, "mtranspose");
	if (mtransposelocation == -1)
	{
		std::cerr << "The mtransposelocation variable doesn't exist in the shader file" << std::endl;
	}

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

	// Use graphic pipeline
	glUseProgram(program);

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::cursorPositionCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		//camera.Update(timer.time.dt);

		glfwPollEvents();

		// clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw call

		pointlight.position = glm::vec4(pointlight.position, 1.0);

		//if (mtx.try_lock())
		{
			glm::mat4 modelmatrix = teapot.mesh->model_pos_mat;
			glm::mat4 mvp = camera.perspective * camera.view * teapot.mesh->model_pos_mat;
			glm::mat3 modelinversetranspose = teapot.mesh->model_vec_mat;
			glm::vec3 camerapos = camera.pos;
			//mtx.unlock();

			glUniformMatrix4fv(mvplocation, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(modelmatrixlocation, 1, GL_FALSE, &modelmatrix[0][0]);
			glUniformMatrix3fv(mtransposelocation, 1, GL_FALSE, &modelinversetranspose[0][0]);
			glUniform3f(cameraposition, camerapos.x, camerapos.y, camerapos.z);
			glUniform3f(ambientintensity, ambientlight.intensity.x, ambientlight.intensity.y, ambientlight.intensity.z);
			glUniform3f(pointintensitylocation, pointlight.intensity.r, pointlight.intensity.g, pointlight.intensity.b);
			glUniform3f(pointpositionlocation, pointlight.position.x, pointlight.position.y, pointlight.position.z);
			glUniform3f(diffuselocation, teapot.mesh->material.Kd[0], teapot.mesh->material.Kd[1], teapot.mesh->material.Kd[2]);
			glUniform4f(specularlocation, teapot.mesh->material.Ks[0], teapot.mesh->material.Ks[1], teapot.mesh->material.Ks[2], teapot.mesh->material.Ns);
			glUniform1i(gSampler, 0);
			glUniform1i(gSampler2, 1);
			glDrawElements(GL_TRIANGLES, teapot.mesh->index.size() * sizeof(teapot.mesh->index[0]), GL_UNSIGNED_INT, (void*)0);

			glfwSwapBuffers(window);

		}
		//else
		{
			//printf("heaven\n");
		}


	}
}
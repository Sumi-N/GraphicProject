#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constatnt.h"
#include "Utility.h"
#include "FileLoader.h"
#include "Camera.h"
#include "Input.h"
#include "Object.h"
#include "Light.h"

GLFWwindow * window;
Camera camera;
Object teapot;
AmbientLight ambientlight;
PointLight pointlight;

int main()
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
	if (glewInit() != GLEW_OK) {
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	// Load teapot data
	teapot.data.LoadFromFileObj("../Objfiles/teapot.obj", true);
	teapot.position = glm::vec3(0, 0, -50);
	//teapot.scale = glm::vec3(1.0, 1.0, 2.0);
	teapot.diffuse = glm::vec3(0.8, 0.2, 0.2);
	teapot.specular = glm::vec4(1.0, 1.0, 1.0, 20);
	teapot.organizeindiceorder();

	// Setup Light
	ambientlight.intensity = glm::vec3(0.1, 0.1, 0.1);
	pointlight.intensity = glm::vec3(1.0, 1.0, 1.0);
	pointlight.position = glm::vec3(20, 0, -50);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	GLuint VBO;
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.data.NV() * sizeof(teapot.data.V(0)), &teapot.data.V(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint NormalBuffer;
	glGenBuffers(1, &NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	//glBufferData(GL_ARRAY_BUFFER, teapot.data.NVN() * sizeof(teapot.data.VN(0)), &teapot.data.VN(0), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, teapot.data.NVN() * sizeof(teapot.sortedvn[0]), &teapot.sortedvn[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint IndexBuffer;
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, teapot.data.NF() * sizeof(teapot.data.F(0)), &teapot.data.F(0), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);


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

	GLint cameramatrixlocation = glGetUniformLocation(program, "cameramatrix");
	if (cameramatrixlocation == -1)
	{
		std::cerr << "The cameramatrix variable doesn't exist in the shader file" << std::endl;
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
		// call callback
		glfwPollEvents();

		// clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw call

		teapot.update();
		glm::mat4 modelmatrix = teapot.modelcoordinate;
		glm::mat4 cameramatrix = camera.view;
		pointlight.position = glm::vec4(pointlight.position, 1.0);
		glm::mat4 mvp = camera.perspective * camera.view * teapot.modelcoordinate;

		glUniformMatrix4fv(mvplocation, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(modelmatrixlocation, 1, GL_FALSE, &modelmatrix[0][0]);
		glUniformMatrix4fv(cameramatrixlocation, 1, GL_FALSE, &cameramatrix[0][0]);
		glUniformMatrix3fv(mtransposelocation, 1, GL_FALSE, &teapot.modelinversetranspose[0][0]);
		glUniform3f(ambientintensity, ambientlight.intensity.x, ambientlight.intensity.y, ambientlight.intensity.z);
		glUniform3f(pointintensitylocation, pointlight.intensity.r, pointlight.intensity.g, pointlight.intensity.b);
		glUniform3f(pointpositionlocation, pointlight.position.x, pointlight.position.y, pointlight.position.z);
		glUniform3f(diffuselocation, teapot.diffuse.r, teapot.diffuse.g, teapot.diffuse.b);
		glUniform4f(specularlocation, teapot.specular.r, teapot.specular.g, teapot.specular.b, teapot.specular.w);
		glDrawElements(GL_TRIANGLES, teapot.data.NF() * sizeof(teapot.data.F(0)), GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers(window);
	}
}
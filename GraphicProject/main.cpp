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

GLFWwindow * window;
Camera camera;
Object teapot;

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

	// Create vertex buffer object
	//data.LoadFromFileObj("../Objfiles/teapot.obj", true);
	teapot.data.LoadFromFileObj("../Objfiles/teapot.obj", true);
	teapot.position = glm::vec3(0, 0, -50);
	//teapot.scale = glm::vec3(1.0, 1.0, 2.0);

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
	glBufferData(GL_ARRAY_BUFFER, teapot.data.NVN() * sizeof(teapot.data.VN(0)), &teapot.data.VN(0), GL_STATIC_DRAW);
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

	GLint mtransposelocation = glGetUniformLocation(program, "mtranspose");
	if (mtransposelocation == -1)
	{
		std::cerr << "The mtransposelocation variable doesn't exist in the shader file" << std::endl;
	}

	// Use graphic pipeline
	glUseProgram(program);

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::cursorPositionCallback);

	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// call callback
		glfwPollEvents();

		// clear window
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Draw call
		camera.updatemvp(teapot);
		glUniformMatrix4fv(mvplocation, 1, GL_FALSE, &camera.mvp[0][0]);
		glUniformMatrix3fv(mtransposelocation, 1, GL_FALSE, &teapot.modeltranspose[0][0]);
		glDrawElements(GL_TRIANGLES, teapot.data.NF() * sizeof(teapot.data.F(0)), GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers(window);
	}
}
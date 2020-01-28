#include <cyCodeBase/cyTriMesh.h>

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

GLFWwindow * window;
cy::TriMesh data;
Camera camera;

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

	//Select OpenGL Version 3.2 Core Profile
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	data.LoadFromFileObj("../Objfiles/teapot.obj", true);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.NV() * sizeof(data.V(0)), &data.V(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


	GLuint IndexBuffer;
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.NF() * sizeof(data.F(0)), &data.F(0), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);


	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint program = FileLoader::loadShaderProgram(VERTEXSHADERPATH, FRAGMENTSHADERPATH);

	GLint mvplocation = glGetUniformLocation(program, "mvp");
	if (mvplocation == -1)
	{
		std::cerr << "The uniform variable doesn't exist in the shader file" << std::endl;
	}

	// Use graphic pipeline
	glUseProgram(program);

	glfwSetKeyCallback(window, Input::keyCallback);
	glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::cursorPositionCallback);

	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// clear window
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Draw call
		glUniformMatrix4fv(mvplocation, 1, GL_FALSE, &camera.mvp[0][0]);
		glDrawElements(GL_TRIANGLES, data.NF() * sizeof(data.F(0)), GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers(window);

		// call callback
		glfwWaitEvents();
	}
}
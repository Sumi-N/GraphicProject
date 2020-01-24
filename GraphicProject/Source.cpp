#include "cyCodeBase/cyTriMesh.h"
#include "math_3d.h"

#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utility.h"
#include "Source.h"
#include "FileLoader.h"
#include "Input.h"

struct  MVPmatrix
{
	Matrix4f mvp;
	Matrix4f model;
	Matrix4f view;
	Matrix4f perspective;
};

GLFWwindow * window;
cy::TriMesh data;
MVPmatrix mvpmatrix;
GLuint program;

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
	window = glfwCreateWindow(800, 800, "Interactive Graphics", NULL, NULL);
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
	glBufferData(GL_ARRAY_BUFFER, data.NVN() * sizeof(data.VN(0)), &data.VN(0), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, data.NV() * sizeof(data.V(0)), &data.V(0), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


	mvpmatrix.model.InitIdentity();
	mvpmatrix.view.InitIdentity();
	mvpmatrix.perspective.InitIdentity();
	mvpmatrix.mvp = mvpmatrix.model * mvpmatrix.view * mvpmatrix.perspective;

	//GLuint UBO;
	//glGenBuffers(1, &UBO);
	//glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4f), &mvpmatrix, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	program = FileLoader::loadShaderProgram("point.vert", "point.frag");
	GLint mvplocation = glGetUniformLocation(program, "mvp");
	if (mvplocation == -1)
	{
		std::cerr << "The uniform variable doesn't exist in the shader file" << std::endl;
	}
	GLfloat matrix[16] = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	// Sets up a key callback
	glfwSetKeyCallback(window, Input::keyCallback);


	// ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// clear window
		glClear(GL_COLOR_BUFFER_BIT);

		// Use graphic pipeline
		glUseProgram(program);
		
		// Draw call
		//glDrawArrays(GL_POINTS, 0, data.NV());
		glUniformMatrix4fv(mvplocation, 1, GL_FALSE, matrix);
		glDrawArrays(GL_POINTS, 0, data.NVN());
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, data.NV());

		glfwSwapBuffers(window);
		// call callback
		glfwWaitEvents();
	}
}
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
	GLFWwindow * const window;

	GLfloat aspect;

public:
	Window(int width = 640, int height = 480, const char *title = "hello") :
		window(glfwCreateWindow(width, height, title, NULL, NULL)) {
		if (window == NULL) {
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if ((glewInit() != GLEW_OK)) {
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}
		glfwSwapInterval(1);

		glfwSetWindowUserPointer(window, this);

		glfwSetWindowSizeCallback(window, resize);
		resize(window, width, height);
	}

	virtual ~Window() {
		glfwDestroyWindow(window);
	}

	int shouldClose() const {
		return glfwWindowShouldClose(window);
	}

	void swapBuffers() {
		glfwSwapBuffers(window);
		glfwWaitEvents();
	}

	GLfloat getAspect() const { return aspect; }

	static void resize(GLFWwindow * const window, int width, int height) {
		glViewport(0, 0, width, height);

		Window * const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) {
			instance->aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		}
	}
};
#pragma once

extern GLFWwindow * window;

namespace Input {
	void key_callback(GLFWwindow * window, int key, int scancode, int action, int modes) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		}
	}
}
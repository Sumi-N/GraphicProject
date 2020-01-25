#pragma once

extern GLFWwindow * window;

struct MouseState
{
	MouseState()
	{
		xpos = 0;
		ypos = 0;
		isRightButtonPressing = false;
		isLeftButtonPressing = false;
	}
	double xpos, ypos;
	bool isRightButtonPressing, isLeftButtonPressing;
};

MouseState mousestate;

namespace Input {

	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				mousestate.isRightButtonPressing = true;
			}
			else if (action == GLFW_RELEASE)
			{
				mousestate.isRightButtonPressing = false;
			}
		}

		if (key == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS)
			{
				mousestate.isLeftButtonPressing = true;
			}
			else if (action == GLFW_RELEASE)
			{
				mousestate.isLeftButtonPressing = false;
			}
		}

	}

	void cursorPositionCallback(GLFWwindow * window, double xpos, double ypos)
	{
		return;
	}
}
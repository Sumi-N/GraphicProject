#pragma once
#include <mutex>
#include "Camera.h"
#include <GLFW/glfw3.h>

extern GLFWwindow * window;
extern GLuint program;
extern DataRenderToGame * BeginSubmittedByRenderThread;

namespace Input {

	struct MouseState
	{
		double xpos, ypos, oldxpos, oldypos;
		bool isRightButtonPressing, isLeftButtonPressing;
		MouseState()
		{
			oldxpos = 0; oldypos = 0;
			xpos = 0; ypos = 0;
			isRightButtonPressing = false;
			isLeftButtonPressing = false;
		}
	};

	MouseState mousestate;

	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) 
	{

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->space = true;
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->up = true;
		}

		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->down = true;
		}

		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->left = true;
		}

		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->right = true;
		}
	}

	void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				mousestate.isRightButtonPressing = true;
			}
			else
			{
				mousestate.isRightButtonPressing = false;
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
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
		mousestate.oldxpos = mousestate.xpos;
		mousestate.oldypos = mousestate.ypos;
		mousestate.xpos = xpos;
		mousestate.ypos = ypos;

		if (mousestate.isRightButtonPressing)
		{
			float rotationratex = (float)(mousestate.xpos - mousestate.oldxpos);
			float rotationratey = (float)(mousestate.ypos - mousestate.oldypos);

			BeginSubmittedByRenderThread->rotationratex = rotationratex;
			BeginSubmittedByRenderThread->rotationratey = rotationratey;
		}

		return;
	}

	void ClearInput()
	{
		BeginSubmittedByRenderThread->up = false;
		BeginSubmittedByRenderThread->down = false;
		BeginSubmittedByRenderThread->right = false;
		BeginSubmittedByRenderThread->left = false;
		BeginSubmittedByRenderThread->space = false;

		BeginSubmittedByRenderThread->rotationratex = 0;
		BeginSubmittedByRenderThread->rotationratey = 0;
	}
}
#pragma once

extern GLFWwindow * window;
extern Camera camera;

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

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(-0.01f, camera.upvector);
		}

		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(0.01f, camera.upvector);
		}

		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(0.01f, camera.rightvector);
		}

		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(-0.01f, camera.rightvector);
		}
	}

	void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
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

		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
			{
				if (!mousestate.isLeftButtonPressing)
				{
					glfwGetCursorPos(window, &mousestate.oldxpos, &mousestate.oldypos);
				}
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
		if (mousestate.isLeftButtonPressing)
		{
			float rotationratex = (float)(xpos - mousestate.oldxpos) / WIDTH;
			float rotationratey = (float)(ypos - mousestate.oldypos) / HEIGHT;

			camera.rotate(-rotationratex, camera.upvector);
			camera.rotate(-rotationratey, camera.rightvector);
		}
		else if (mousestate.isRightButtonPressing)
		{

		}

		return;
	}
}
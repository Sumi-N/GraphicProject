#pragma once

extern GLFWwindow * window;
extern Camera camera;
extern GLuint program;

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

		if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
		{
			FileLoader::recompileShader();
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(0.01f, camera.forwardvector);
		}

		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translate(-0.01f, camera.forwardvector);
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
				glfwGetCursorPos(window, &mousestate.oldxpos, &mousestate.oldypos);
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

			if (rotationratex > 0)
			{
				camera.rotate(1, camera.upvector);
			}
			else if (rotationratey < 0)
			{
				camera.rotate(-1, camera.upvector);
			}
			//camera.rotate(-rotationratey, camera.rightvector);

			if (rotationratey > 0)
			{
				camera.rotate(1, camera.rightvector);
			}
			else if (rotationratey < 0)
			{
				camera.rotate(-1, camera.rightvector);
			}
		}
		else if (mousestate.isRightButtonPressing)
		{

		}

		return;
	}
}
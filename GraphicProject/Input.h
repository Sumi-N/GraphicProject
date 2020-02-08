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
		glm::vec3 zero = glm::vec3(0, 0, 0);
		camera.MoveCamera(zero);

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
			glm::vec3 forward = 0.01f * camera.forwardvec;
			camera.MoveCamera(forward);
		}

		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			glm::vec3 back = -0.01f * camera.forwardvec;
			camera.MoveCamera(back);
		}

		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			glm::vec3 right = 0.01f * camera.rightvec;
			camera.MoveCamera(right);
		}

		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			glm::vec3 left = -0.01f * camera.rightvec;
			camera.MoveCamera(left);
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

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = camera.rightvec;

		if (mousestate.isRightButtonPressing)
		{
			float rotationratex = (float)(mousestate.xpos - mousestate.oldxpos);
			float rotationratey = (float)(mousestate.ypos - mousestate.oldypos);

			if (rotationratex > 0)
			{
				camera.RotateAround(1, up);
			}			
			else if (rotationratex < 0)
			{
				camera.RotateAround(-1, up);
			}

			if (rotationratey > 0)
			{
				camera.RotateAround(1, right);
			}
			else if (rotationratey < 0)
			{
				camera.RotateAround(-1, right);
			}
		}

		return;
	}
}
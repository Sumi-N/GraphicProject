#pragma once
#include <mutex>
#include "Camera.h"
#include "FileLoader.h"
#include <GLFW/glfw3.h>

extern GLFWwindow * window;
extern Camera camera;
extern GLuint program;
extern DataRequiredForGameThread * BeginSubmittedByRenderThread;

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
		camera.MoveCamera(0, zero);

		glm::vec3 up = camera.forwardvec;
		glm::vec3 right = camera.rightvec;

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
			//camera.MoveCamera(0.01f, up);
			BeginSubmittedByRenderThread->up = true;
			//camera.Translate(camera.pos + 0.5f * up);
		}

		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->down = true;
			//camera.Translate(camera.pos - 0.5f * up);
		}

		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->left = true;
			//camera.Translate(camera.pos - 0.5f * right);
		}

		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			BeginSubmittedByRenderThread->right = true;
			//camera.Translate(camera.pos + 0.5f * right);
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

	enum KeyCodes
	{
		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,

		Space = 0x20,

		Escape = 0x1b,

		Shift = 0x10,
		Control = 0x11,
		Alt = 0x12,

		Tab = 0x09,
		CapsLock = 0x14,

		BackSpace = 0x08,
		Enter = 0x0d,
		Delete = 0x2e,

		PageUp = 0x21,
		PageDown = 0x22,
		End = 0x23,
		Home = 0x24,

		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7a,
		F12 = 0x7b,

		A = 0x41,
		W = 0x57,
		S = 0x53,
		D = 0x44,
		Q = 0x51,
		E = 0x45,

		MOUSELEFT = 0x01,
		MOUSERIGHT = 0x02,
	};

	bool IsKeyPressed(const uint_fast8_t i_keyCode);
	bool IsKeyReleased(const uint_fast8_t i_keyCode);
}
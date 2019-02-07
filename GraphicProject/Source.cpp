#include "math_3d.h"
#include "cyCodeBase/cyTriMesh.h"

#include <cstdlib>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
	// Initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		// 初期化に失敗した
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//Register a function which will execute when the program finied 
	atexit(glfwTerminate);

	//Select OpenGL Version 3.2 Core Profile
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating a window
	GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello!", NULL, NULL));
	if (window == NULL)
	{
		// ウィンドウが作成できなかった
		std::cerr << "Can't create GLFW window." << std::endl;
		glfwTerminate();
		return 1;
	}
	// 作成したウィンドウを OpenGL の処理対象にする
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	std::ostream * pipe = new std::ostream();
	//std::ofstream * pipe;
	
	cy::TriMesh data;
	data.LoadFromFileObj("teapot", true, pipe);

	Vector3f Vertices[3241];
	Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);
		//
		// ここで描画処理を行う
		glDrawArrays(GL_POINTS, 0, 1);
		//
		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
		// イベントを取り出す
		glfwWaitEvents();
	}
}
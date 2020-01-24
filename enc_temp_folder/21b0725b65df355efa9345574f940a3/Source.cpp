#include "cyCodeBase/cyTriMesh.h"
#include "math_3d.h"

#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

// シェーダオブジェクトのコンパイル結果を表示する
// shader: シェーダオブジェクト名
// str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
	// コンパイル結果を取得する
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
	// シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// シェーダのコンパイル時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

// プログラムオブジェクトのリンク結果を表示する
// program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program)
{
	// リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
	// シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
		// シェーダのリンク時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

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


	//mvpmatrix.model.InitIdentity();
	//mvpmatrix.view.InitIdentity();
	//mvpmatrix.perspective.InitIdentity();
	//mvpmatrix.mvp = mvpmatrix.model * mvpmatrix.view * mvpmatrix.perspective;

	//GLuint UBO;
	//glGenBuffers(1, &UBO);
	//glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4f), &mvpmatrix, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//// バーテックスシェーダのソースプログラム
	//static constexpr GLchar vsrc[] =
	//	"#version 330 core\n"
	//	"in vec4 position;\n"
	//	"void main()\n"
	//	"{\n"
	//	" float s = 1.0;\n"
	//	" gl_Position = s * position;\n"
	//	"}\n";
	//// フラグメントシェーダのソースプログラム
	//static constexpr GLchar fsrc[] =
	//	"#version 330 core\n"
	//	"out vec4 fragment;\n"
	//	"void main()\n"
	//	"{\n"
	//	" fragment = vec4(1.0, 1.0, 0.0, 1.0);\n"
	//	"}\n";

	const GLuint program(FileLoader::loadShaderProgram("point.vert", "point.frag"));
	const GLint aspectLoc(glGetUniformLocation(program, "aspect"));

	// Sets up a key callback
	glfwSetKeyCallback(window, Input::keyCallback);


	// ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用開始
		glUseProgram(program);
		
		// ここで描画処理を行う
		//glDrawArrays(GL_POINTS, 0, data.NV());
		glDrawArrays(GL_POINTS, 0, data.NVN());
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, data.NV());


		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
		// call callback
		glfwWaitEvents();
	}
}
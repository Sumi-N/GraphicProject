#include "cyCodeBase/cyTriMesh.h"

#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source.h"
#include "Input.h"

GLFWwindow * window;

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

GLuint createProgram(const char *vsrc, const char *fsrc)
{
	// 空のプログラムオブジェクトを作成する
	const GLuint program(glCreateProgram());
	if (vsrc != NULL)
	{
		// バーテックスシェーダのシェーダオブジェクトを作成する
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);
		// バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}
	if (fsrc != NULL)
	{
		// フラグメントシェーダのシェーダオブジェクトを作成する
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);
		// フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(fobj, "fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}
	// プログラムオブジェクトをリンクする
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);
	// 作成したプログラムオブジェクトを返す
	if (printProgramInfoLog(program))
		return program;
	glDeleteProgram(program);
	return 0;
}

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
	window = glfwCreateWindow(800, 800, "Interactive Graphics", NULL, NULL);
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

	//Vertex shader pipeline?	
	cy::TriMesh data;
	data.LoadFromFileObj("../Objfiles/teapot.obj", true);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.NVN() * sizeof(data.VN(0)), &data.VN(0), GL_STATIC_DRAW);
	//std::cout << data.NV() * sizeof(data.V(0)) << std::endl;
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	//GLuint vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// バーテックスシェーダのソースプログラム
	static constexpr GLchar vsrc[] =
		"#version 150 core\n"
		"in vec4 position;\n"
		"void main()\n"
		"{\n"
		" float s = 1.0;\n"
		" gl_Position = s * position;\n"
		"}\n";
	// フラグメントシェーダのソースプログラム
	static constexpr GLchar fsrc[] =
		"#version 150 core\n"
		"out vec4 fragment;\n"
		"void main()\n"
		"{\n"
		" fragment = vec4(1.0, 1.0, 0.0, 1.0);\n"
		"}\n";
	// プログラムオブジェクトを作成する
	const GLuint program(createProgram(vsrc, fsrc));

	// Sets up a key callback
	glfwSetKeyCallback(window, Input::key_callback);


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
		//
		// カラーバッファを入れ替える
		glfwSwapBuffers(window);
		// イベントを取り出す
		glfwWaitEvents();
	}
}
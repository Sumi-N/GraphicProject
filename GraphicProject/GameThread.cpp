#include "GameThread.h"
#include <mutex>
#include <stdio.h>

//#include "FileLoader.h"
#include "Object.h"
#include "Camera.h"

extern Object teapot;
extern Camera camera;
extern Timer  timer;
extern std::mutex mtx;

namespace Application {

	GameThread::GameThread()
	{
	}


	GameThread::~GameThread()
	{
	}

	void GameThread::Init()
	{
		timer.Init();
	}

	void GameThread::Run()
	{
		while (true)
		{
			timer.Run();
			teapot.update();
			camera.Update(timer.time.dt);
		}
	}

	int Init()
	{
		printf("I start the other thread\n");

		// Load teapot data
		teapot.data.LoadFromFileObj("../Objfiles/teapot.obj", true);
		teapot.pos = glm::vec3(0, 0, -50);
		//teapot.scale = glm::vec3(1.0, 1.0, 2.0);
		//Texture * pottexture = FileLoader::ReadTexture("../Objfiles/brick.png");

		teapot.organizeindiceorder();

		GameThread gamethread;

		gamethread.Init();
		gamethread.Run();
	
		return 0;
	}
}

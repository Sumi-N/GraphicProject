#include "GameThread.h"
#include <stdio.h>

#include "Object.h"
#include "Camera.h"

extern Object teapot;
extern Camera camera;

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
		teapot.diffuse = glm::vec3(0.8, 0.2, 0.2);
		teapot.specular = glm::vec4(1.0, 1.0, 1.0, 20);
		teapot.organizeindiceorder();

		GameThread gamethread;
		gamethread.Init();
		gamethread.Run();
	
		return 0;
	}
}

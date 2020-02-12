#pragma once

#include "GameThread.h"
#include <mutex>
#include <stdio.h>

#include "Material.h"
#include "Object.h"
#include "Camera.h"
#include "FileLoader.h"

extern Object teapot;
extern Camera camera;

struct DataRequiredForBuffer
{
	std::vector<Camera> camera;
	std::vector<Mesh> meshlist;
	std::vector<Texture> textureList;
	std::vector<Material> materialList;
};

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
			teapot.mesh->Update();
			camera.Update(timer.time.dt);

		}
	}

namespace Application {

	int Init()
	{
		printf("I start the other thread\n");

		GameThread gamethread;

		gamethread.Init();
		gamethread.Run();
	
		return 0;
	}
}

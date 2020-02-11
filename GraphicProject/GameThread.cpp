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
			teapot.mesh->Update();
			camera.Update(timer.time.dt);
		}
	}

	int Init()
	{
		printf("I start the other thread\n");

		// Load teapot data
		teapot.mesh = new Mesh();
		teapot.mesh->Load("../Objfiles/teapot.obj");
		teapot.mesh->Init();
		teapot.mesh->texture = new Texture();
		teapot.mesh->texture->Load("../Objfiles/brick.png");

		// Setting up position 
		teapot.pos = glm::vec3(0, 0, -50);
		teapot.scale = glm::vec3(1.0, 1.0, 2.0);
		teapot.rot = glm::vec3(90, 0, 0);


		GameThread gamethread;

		gamethread.Init();
		gamethread.Run();
	
		return 0;
	}
}

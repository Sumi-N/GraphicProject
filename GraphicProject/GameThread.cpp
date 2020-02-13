#pragma once

#include "GameThread.h"
#include <mutex>
#include <stdio.h>

#include "Material.h"
#include "Object.h"
#include "Camera.h"
#include "FileLoader.h"
#include "main.h"

extern Object teapot;
extern Camera camera;
extern DataRequiredForGameThread * BeginReadByGameThread;

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

			if (BeginReadByGameThread->up)
			{
				camera.MoveCamera(0.01f, camera.forwardvec);
				//camera.Translate(camera.pos + 0.5f * camera.forwardvec);
			}

			if (BeginReadByGameThread->down)
			{
				camera.MoveCamera(-0.01f, camera.forwardvec);
				//camera.Translate(camera.pos - 0.5f * camera.forwardvec);
			}

			if (BeginReadByGameThread->left)
			{
				camera.MoveCamera(-0.01f, camera.rightvec);
				//camera.Translate(camera.pos - 0.5f * camera.rightvec);
			}

			if (BeginReadByGameThread->right)
			{
				camera.MoveCamera(0.01f, camera.rightvec);
				//camera.Translate(camera.pos + 0.5f * camera.rightvec);
			}

			// Check if render thread is ready to get date from game thread
			bool canSubmitDataToRenderThread;
			do
			{
				canSubmitDataToRenderThread = WaitUntilDataCanSubmitFromApplicationThread(250);
			} while (!canSubmitDataToRenderThread);

			{
				// Submit data in this scope
			}

			SignalTheDataHasBeenSubmitted();
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

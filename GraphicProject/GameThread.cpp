#pragma once

#include "GameThread.h"
#include <mutex>
#include <stdio.h>

#include "Material.h"
#include "Object.h"
#include "Quad.h"
#include "Camera.h"
#include "main.h"

extern Object teapot;
extern Quad quad;
extern Camera camera;
extern DataRequiredForGameThread * BeginReadByGameThread;
extern DataRequiredForGameThread * BeginSubmittedByRenderThread;

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
			if (BeginReadByGameThread->up)
			{
				camera.MoveCamera(0.01f, camera.forwardvec);
			}

			if (BeginReadByGameThread->down)
			{
				camera.MoveCamera(-0.01f, camera.forwardvec);
			}

			if (BeginReadByGameThread->left)
			{
				camera.MoveCamera(-0.01f, camera.rightvec);
			}

			if (BeginReadByGameThread->right)
			{
				camera.MoveCamera(0.01f, camera.rightvec);
			}

			timer.Run();
			teapot.mesh->Update();
			quad.mesh->Update();
			camera.Update(timer.time.dt);

			// Check if render thread is ready to get date from game thread
			bool canSubmitDataToRenderThread;
			do
			{
				canSubmitDataToRenderThread = WaitUntilDataCanSubmitFromApplicationThread(250);
			} while (!canSubmitDataToRenderThread);

			{
				// Submit data in this scope
				SubmitObjectData(&teapot);
				//SubmitObjectData(&quad);
				SubmitCameraData(&camera);
				SubmitLightingData();
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

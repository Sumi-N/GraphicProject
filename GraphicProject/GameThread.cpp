#pragma once

#include "GameThread.h"
#include "RenderThread.h"

#include "Material.h"
#include "Object.h"
#include "Quad.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Light.h"

extern DataRenderToGame * BeginReadByGameThread;
extern DataRenderToGame * BeginSubmittedByRenderThread;

Camera camera;
Object teapot;
AmbientLight ambientlight;
PointLight pointlight;
Quad quad;
CubeMap cubemap;

	GameThread::GameThread()
	{
	}


	GameThread::~GameThread()
	{
	}

	void GameThread::Init()
	{
		// Initialize timer
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
				SubmitObjectData(&quad);
				SubmitCameraData(&camera);
				SubmitLightingData();
			}

			SignalTheDataHasBeenSubmitted();
		}
	}

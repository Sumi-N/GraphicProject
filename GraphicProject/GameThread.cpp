#pragma once

#include "GameThread.h"
#include "RenderThread.h"

#include "Material.h"
#include "Object.h"
#include "Quad.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Light.h"

extern RenderThread renderthread;
extern std::mutex mutex;
extern std::condition_variable condition_variable;

DataRenderToGame dataRequiredForGameThread[2];
DataRenderToGame * BeginSubmittedByRenderThread = &dataRequiredForGameThread[0];
DataRenderToGame * BeginReadByGameThread = &dataRequiredForGameThread[1];

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
			canSubmitDataToRenderThread = renderthread.WaitUntilDataCanSubmitFromApplicationThread(250);
		} while (!canSubmitDataToRenderThread);

		{
			// Submit data in this scope
			renderthread.SubmitObjectData(&teapot);
			renderthread.SubmitObjectData(&quad);
			renderthread.SubmitCameraData(&camera);
			renderthread.SubmitLightingData();
		}

		renderthread.SignalTheDataHasBeenSubmitted();
	}
}

void GameThread::RenderToGameInfo()
{
	std::swap(BeginReadByGameThread, BeginSubmittedByRenderThread);
}

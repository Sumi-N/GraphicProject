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
extern std::mutex mutex_game;
extern std::mutex mutex_render;
extern std::condition_variable cv_game;
extern std::condition_variable cv_render;
extern bool brenderready;
extern bool bgameready;

DataRenderToGame datarendertogame[2];
DataRenderToGame * BeginSubmittedByRenderThread = &datarendertogame[0];
DataRenderToGame * BeginReadByGameThread = &datarendertogame[1];

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
		if (BeginReadByGameThread->space)
		{
			glm::vec3 zero = glm::vec3(0, 0, 0);
			camera.MoveCamera(0, zero);
		}

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = camera.rightvec;

		if (BeginReadByGameThread->rotationratex > 0)
		{
			camera.RotateAround(1, up);
		}
		else if (BeginReadByGameThread->rotationratex < 0)
		{
			camera.RotateAround(-1, up);
		}

		if (BeginReadByGameThread->rotationratey > 0)
		{
			camera.RotateAround(1, right);
		}
		else if (BeginReadByGameThread->rotationratey < 0)
		{
			camera.RotateAround(-1, right);
		}

		timer.Run();
		teapot.Update(timer.time.dt);
		quad.Update(timer.time.dt);
		camera.Update(timer.time.dt);

		{
			std::unique_lock<std::mutex> unique_lock_guard(mutex_render);
			while (!brenderready)
				cv_render.wait(unique_lock_guard);

			{
				// Submit data in this scope
				renderthread.SubmitObjectData(&teapot);
				renderthread.SubmitImageData(&quad);
				renderthread.SubmitCameraData(&camera);
				renderthread.SubmitLightingData();
			}
			brenderready = false;
		}
	}
}

void GameThread::RenderToGameInfo()
{
	std::swap(BeginReadByGameThread, BeginSubmittedByRenderThread);
}

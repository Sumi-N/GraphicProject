#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "RenderThread.h"
#include "GameThread.h"

// Those are the events to signal another thread that something is done
std::mutex mutex_game;
std::mutex mutex_render;
std::condition_variable cv_game;
std::condition_variable cv_render;
bool brenderready = false;
bool bgameready = false;

GameThread gamethread;
RenderThread renderthread;

int main2()
{
	{
		std::unique_lock<std::mutex> unique_lock_guard(mutex_render);
		while(!brenderready)
			cv_render.wait(unique_lock_guard);
		brenderready = false;
	}

	{
		std::lock_guard<std::mutex> lock_guard(mutex_game);
		gamethread.Init();
		bgameready = true;
		cv_game.notify_one();
	}

	gamethread.Run();
	return 0;
}

int main()
{
	// Start game thread
	std::thread gamethread(main2);

	// Start render thread
	{
		{
			std::lock_guard<std::mutex> lock_guard(mutex_render);
			renderthread.Init();
			brenderready = true;
			cv_render.notify_one();
		}

		{
			std::unique_lock<std::mutex> unique_lock_guard(mutex_game);
			while(!bgameready)
				cv_game.wait(unique_lock_guard);
			bgameready = false;
		}

		renderthread.Run();
		return 0;
	}
}
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "RenderThread.h"
#include "GameThread.h"
#include "Event.h"

// Those are the events to signal another thread that something is done
Event event_done_submitting_from_game;
Event event_can_submit_from_game;

std::mutex mutex;
std::mutex mutex2;
std::condition_variable condition_variable;

GameThread gamethread;
RenderThread renderthread;

int main2()
{
	{
		std::unique_lock<std::mutex> unique_lock_guard(mutex);
		condition_variable.wait(unique_lock_guard);
	}

	{
		std::lock_guard<std::mutex> lock_guard(mutex);
		gamethread.Init();
		condition_variable.notify_one();
	}

	gamethread.Run();
	return 0;
}

int main()
{

	event_done_submitting_from_game.Init(EventType::ResetAutomatically);
	event_can_submit_from_game.Init(EventType::ResetAutomatically, EventState::Signaled);

	// Start game thread
	std::thread gamethread(main2);

	// Start render thread
	{
		{
			std::lock_guard<std::mutex> lock_guard(mutex);
			renderthread.Init();
			condition_variable.notify_one();
		}

		{
			std::unique_lock<std::mutex> unique_lock_guard(mutex);
			condition_variable.wait(unique_lock_guard);
		}

		renderthread.Run();
		return 0;
	}
}
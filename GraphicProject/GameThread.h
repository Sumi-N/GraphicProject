#pragma once

#include "Timer.h"

class GameThread
{
public:
	GameThread();
	~GameThread();

	Timer timer;

	void Init();
	void Run();
};




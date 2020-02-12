#pragma once

#include "Timer.h"

namespace Application {
	int Init();
}

class GameThread
{
public:
	GameThread();
	~GameThread();

	Timer timer;

	void Init();
	void Run();
};




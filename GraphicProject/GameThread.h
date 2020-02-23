#pragma once

#include "Timer.h"

struct DataRenderToGame
{
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
};

class GameThread
{
public:
	GameThread();
	~GameThread();

	Timer timer;

	void Init();
	void Run();
	void RenderToGameInfo();
};




#pragma once

#include "Timer.h"

namespace Application {
	class GameThread
	{	
	public:
		GameThread();
		~GameThread();

		Timer timer;

		void Init();
		void Run();
	};

	int Init();
}


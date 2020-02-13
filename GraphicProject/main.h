#pragma once

bool WaitUntilDataCanSubmitFromApplicationThread(const double i_timetowait);

void SignalTheDataHasBeenSubmitted();

struct DataRequiredForGameThread
{
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
};

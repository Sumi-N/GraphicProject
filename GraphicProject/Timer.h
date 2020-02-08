#pragma once

#include <Windows.h>

class Timer {
public:

	struct Time {
		double frequencypermillisecond;
		LARGE_INTEGER lpFrequency;
		LARGE_INTEGER lpPerformanceCount_begin;
		LARGE_INTEGER lpPerformanceCount_end;
		double dt;
	};

	Time time;

	inline void Init()
	{
		QueryPerformanceFrequency(&time.lpFrequency);
		time.frequencypermillisecond = double(time.lpFrequency.QuadPart) / 1000;

		time.lpPerformanceCount_begin.QuadPart = 0;
	}

	inline void Run()
	{
		QueryPerformanceCounter(&time.lpPerformanceCount_end);
		if (time.lpPerformanceCount_begin.QuadPart == 0)
		{
			time.dt = 0;
		}
		else
		{
			time.dt = double(time.lpPerformanceCount_end.QuadPart - time.lpPerformanceCount_begin.QuadPart) / time.frequencypermillisecond;
		}
		time.lpPerformanceCount_begin.QuadPart = time.lpPerformanceCount_end.QuadPart;
	}
};
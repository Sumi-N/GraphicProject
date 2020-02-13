#pragma once
#include <windows.h>

enum class EventType
{
	ResetAutomatically,
	ResetManually,
};

enum class EventState
{
	Unsignaled,
	Signaled,
};

class Event
{
public:
	Event();
	~Event();

	void Initialize(const EventType i_type, const EventState i_initialstate = EventState::Unsignaled);
	bool Signal();
	void ResetToUnsignaled();

	HANDLE m_handle = NULL;
};

bool WaitForEvent(const Event & i_event, const double i_timetowait);

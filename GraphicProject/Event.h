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

	void Signal();
	void ResetToUnsignaled();

	HANDLE m_handle = NULL;
};


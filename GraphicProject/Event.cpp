#include "Event.h"
#include <stdio.h>

bool WaitForEvent(const Event & i_event, const double i_timetowait)
{
	if (i_event.m_handle)
	{
		DWORD result = WaitForSingleObject(i_event.m_handle, i_timetowait);
		switch (result)
		{
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		default:
			printf("something went wrong with wait for event function");
			return false;
		}
	}
}

Event::Event()
{
}


Event::~Event()
{
}

void Event::Init(const EventType i_type, const EventState i_initialState)
{
	m_handle = CreateEventW(nullptr, i_type == EventType::ResetManually, i_initialState == EventState::Signaled, nullptr);
}

bool Event::Signal()
{
	if (SetEvent(m_handle) != FALSE)
	{
		return true;
	}
}

#include "Event.h"



Event::Event()
{
}


Event::~Event()
{
}

void Event::Initialize(const EventType i_type, const EventState i_initialState)
{
	m_handle = CreateEventW(nullptr, i_type == EventType::ResetManually, i_initialState == EventState::Signaled, nullptr);
}

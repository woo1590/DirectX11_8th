#include "EnginePCH.h"
#include "EventSystem.h"

EventSystem::EventSystem()
{
}

EventSystem* EventSystem::Create(_uint numEvents)
{
	EventSystem* Instance = new EventSystem();

	if (FAILED(Instance->Intialize(numEvents)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EventSystem::Intialize(_uint numEvents)
{
	m_Listeners.resize(numEvents);

	return S_OK;
}

void EventSystem::Update(_float dt)
{
	_uint currEvents = m_Events.size();

	for (_uint i = 0; i < currEvents; ++i)
	{
		if (m_Events.empty())
			break;

		auto& event = m_Events.front();

		for (const auto& listener : m_Listeners[event.eventID])
			listener.eventFunction(event.param);

		m_Events.pop();
	}

	RemoveListener();
}

void EventSystem::PublishEvent(_uint eventID, std::any param)
{
	m_Events.push({ eventID,param });
}

_int EventSystem::Subscribe(_uint eventID, const LISTENER& listener)
{
	if (eventID >= m_Listeners.size())
		return -1;

	m_Listeners[eventID].push_back(listener);

	return m_iID++;
}

void EventSystem::UnSubscribe(_uint eventID, const EventHandler* listener)
{
	if (eventID >= m_Listeners.size())
		return;

	m_RemoveHandler.push_back(listener);
}

void EventSystem::UnSubscribeById(_uint id)
{
	m_RemoveIds.push_back(id);
}

void EventSystem::Free()
{
	__super::Free();
}

void EventSystem::RemoveListener()
{
	if (m_RemoveHandler.empty())
		return;

	for (auto& handler : m_RemoveHandler)
	{
		for (_uint i = 0; i < m_Listeners.size(); ++i)
		{
			for (auto iter = m_Listeners[i].begin(); iter != m_Listeners[i].end();)
			{
				if ((*iter).handler == handler)
					iter = m_Listeners[i].erase(iter);
				else
					++iter;
			}
		}
	}
}

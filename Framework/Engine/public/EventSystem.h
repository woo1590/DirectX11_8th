#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class EventHandler;
class EventSystem :
    public Base
{
public:

private:
    EventSystem();
    virtual ~EventSystem() = default;

public:
    static EventSystem* Create(_uint numEvents);
    HRESULT Intialize(_uint numEvents);
    void Update(_float dt);

    void PublishEvent(_uint eventID, std::any param = {});
    _int Subscribe(_uint eventID, const LISTENER& listener);
    void UnSubscribe(_uint eventID, const EventHandler* listener);
    void UnSubscribeById(_uint id);

    void Free()override;
    
private:
    void RemoveListener();

    _uint m_iID{};
    std::queue<EVENT> m_Events;
    std::vector<std::vector<LISTENER>> m_Listeners;

    std::vector<const EventHandler*> m_RemoveHandler;
    std::vector<_uint> m_RemoveIds;
};

NS_END
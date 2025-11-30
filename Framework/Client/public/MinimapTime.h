#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class MinimapTime :
    public UIObject
{
private:
    MinimapTime();
    MinimapTime(const MinimapTime& prototype);
    virtual ~MinimapTime() = default;

public:
    static MinimapTime* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector < std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
    _uint m_iMinute{};
    _uint m_iSecond{};
    _wstring m_strText{};
};

NS_END
#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class MinimapLevel :
    public UIObject
{
private:
    MinimapLevel();
    MinimapLevel(const MinimapLevel& prototype);
    virtual ~MinimapLevel() = default;

public:
    static MinimapLevel* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    HRESULT LateInitialize()override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _wstring m_strStageTag{};
};

NS_END
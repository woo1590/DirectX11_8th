#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class CoolDownEffect :
    public UIObject
{
private:
    CoolDownEffect();
    CoolDownEffect(const CoolDownEffect& prototype);
    virtual ~CoolDownEffect() = default;

public:
    static CoolDownEffect* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
};

NS_END
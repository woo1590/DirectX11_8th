#pragma once
#include "Object.h"

NS_BEGIN(Client)

class ItemGlow :
    public Object
{
private:
    ItemGlow();
    ItemGlow(const ItemGlow& prototype);
    virtual ~ItemGlow() = default;

public:
    static ItemGlow* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float2 m_UVOffset{};
};

NS_END
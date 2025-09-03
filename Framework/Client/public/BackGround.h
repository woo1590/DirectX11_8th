#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class BackGround final:
    public UIObject
{
private:
    BackGround();
    virtual ~BackGround() = default;

public:
    static BackGround* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    VIBuffer* m_pVIBuffer = nullptr;
    Material* m_pMaterial = nullptr;
    _uint m_iTexNum = 0;

};

NS_END
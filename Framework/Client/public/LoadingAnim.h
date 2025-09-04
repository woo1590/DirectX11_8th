#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class LoadingAnim :
    public UIObject
{
private:
    LoadingAnim();
    LoadingAnim(const LoadingAnim& prototype);
    virtual ~LoadingAnim() = default;

public:
    static LoadingAnim* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    VIBuffer* m_pVIBuffer = nullptr;
    Material* m_pMaterial = nullptr;
    _uint m_iTexNum = 0;
    _float m_fElaspedTime = 0.f;
};

NS_END
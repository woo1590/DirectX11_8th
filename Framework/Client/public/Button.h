#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class Button :
    public UIObject
{
private:
    Button();
    Button(const Button& prototype);
    virtual ~Button() = default;

public:
    static Button* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT DrawButton();
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    void CheckCollision();

    _bool m_IsHover = false;
    _float m_fElapsedTime{};
    _float m_fDuration = 0.2f;

    _bool m_IsActive = false;
    _float m_fActiveElapsedTime{};
    _float m_fActiveDuration = 0.2f;

    VIBuffer* m_pBuffer = nullptr;
    Material* m_pMaterial = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;

    std::vector<_float2> m_Offsets{};
    _float4 m_FontColor{};
    _float3 m_TargetScale{};
    _float3 m_StartScale{};
};

NS_END
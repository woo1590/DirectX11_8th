#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BarrelBoom :
    public Object
{
private:
    BarrelBoom();
    BarrelBoom(const BarrelBoom& prototype);
    virtual ~BarrelBoom() = default;

public:
    static BarrelBoom* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
    _float m_fDuration = 0.2f;
    _float4 m_StartLightColor{ 1.f,0.4f,0.f,1.f };
    _float4 m_EndLightColor{ 0.5f,0.1f,0.f,0.f };
};

NS_END
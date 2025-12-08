#pragma once
#include "Object.h"

NS_BEGIN(Client)

class ShieldHit :
    public Object
{
private:
    ShieldHit();
    ShieldHit(const ShieldHit& prototype);
    virtual ~ShieldHit() = default;

public:
    static ShieldHit* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
    _float m_fDuration = 0.15f;

    _float3 m_StartScale{};
    _float3 m_EndScale{};
    _float m_fStartAlpha{};
    _float m_fEndAlpha{};

};

NS_END
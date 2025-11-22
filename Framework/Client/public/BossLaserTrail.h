#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossLaserTrail :
    public Object
{
private:
    BossLaserTrail();
    BossLaserTrail(const BossLaserTrail& prototype);
    virtual ~BossLaserTrail() = default;

public:
    static BossLaserTrail* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void SetLaserPoints(_float3 p0, _float3 p1);

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
};

NS_END
#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossPillarSmoke :
    public Object
{
public:
    typedef struct tagBossPillarSmokeDesc : public Object::OBJECT_DESC
    {
        _float duration{};
    }BOSS_PILLAR_SMOKE_DESC;
private:
    BossPillarSmoke();
    BossPillarSmoke(const BossPillarSmoke& prototype);
    virtual ~BossPillarSmoke() = default;

public:
    static BossPillarSmoke* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
    
private:
    _float m_fElapsedTime{};
    _float m_fDurtaion{};

};

NS_END
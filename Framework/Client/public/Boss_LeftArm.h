#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Boss_LeftArm :
    public PartObject
{
public:
    typedef struct tagBossLeftArmDesc : public PartObject::PART_OBJECT_DESC
    {

    }BOSS_LEFT_ARM_DESC;
private:
    Boss_LeftArm();
    Boss_LeftArm(const Boss_LeftArm& prototype);
    virtual ~Boss_LeftArm() = default;

public:
    static Boss_LeftArm* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* other)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

};

NS_END
#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Boss_RightArm :
    public PartObject
{
public:
    typedef struct tagBossRightArmDesc : public PartObject::PART_OBJECT_DESC
    {

    }BOSS_RIGHT_ARM_DESC;
private:
    Boss_RightArm();
    Boss_RightArm(const Boss_RightArm& prototype);
    virtual ~Boss_RightArm() = default;

public:
    static Boss_RightArm* Create();
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
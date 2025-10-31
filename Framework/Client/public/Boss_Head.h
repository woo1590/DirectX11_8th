#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Boss_Head :
    public PartObject
{
public:
    typedef struct tagBossHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }BOSS_HEAD_DESC;
private:
    Boss_Head();
    Boss_Head(const Boss_Head& prototype);
    virtual ~Boss_Head() = default;

public:
    static Boss_Head* Create();
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
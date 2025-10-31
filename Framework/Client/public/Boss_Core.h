#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Boss_Core :
    public PartObject
{
public:
    typedef struct tagBossCoreDesc : public PartObject::PART_OBJECT_DESC
    {

    }BOSS_CORE_DESC;
private:
    Boss_Core();
    Boss_Core(const Boss_Core& prototype);
    virtual ~Boss_Core() = default;

public:
    static Boss_Core* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    void OnCollisionEnter(ColliderComponent* other)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    
};

NS_END
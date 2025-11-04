#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Soldier_Sword :
    public PartObject
{
public:
    typedef struct tagSoldierSwordDesc : public PartObject::PART_OBJECT_DESC
    {

    }SOLDIER_SWORD_DESC;
private:
    Soldier_Sword();
    Soldier_Sword(const Soldier_Sword& prototype);
    virtual ~Soldier_Sword() = default;

public:
    static Soldier_Sword* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
};

NS_END
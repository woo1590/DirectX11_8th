#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class HorseHead_Sword :
    public PartObject
{
public:
    typedef struct tagHorseHeadSwordDesc : PartObject::PART_OBJECT_DESC
    {

    }HORSE_HEAD_SWORD_DESC;
private:
    HorseHead_Sword();
    HorseHead_Sword(const HorseHead_Sword& prototype);
    virtual ~HorseHead_Sword() = default;

public:
    static HorseHead_Sword* Create(); 
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class HorseHead_Shield :
    public PartObject
{
public:
    typedef struct tagHorseHeadShieldDesc : public PartObject::PART_OBJECT_DESC
    {

    }HORSE_HEAD_SHIELD_DESC;
private:
    HorseHead_Shield();
    HorseHead_Shield(const HorseHead_Shield& prototype);
    virtual ~HorseHead_Shield() = default;

public:
    static HorseHead_Shield* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
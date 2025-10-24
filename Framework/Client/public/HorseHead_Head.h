#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class HorseHead_Head :
    public PartObject
{
public:
    typedef struct tagHorseHeadHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }HORSE_HEAD_HEAD_DESC;
private:
    HorseHead_Head();
    HorseHead_Head(const HorseHead_Head& prototype);
    virtual ~HorseHead_Head() = default;

public:
    static HorseHead_Head* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
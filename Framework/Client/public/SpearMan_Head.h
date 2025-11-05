#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class SpearMan_Head :
    public PartObject
{
public:
    typedef struct tagSpearManHeadDesc : public PartObject::PART_OBJECT_DESC
    {

    }SPEAR_MAN_HEAD_DESC;
private:
    SpearMan_Head();
    SpearMan_Head(const SpearMan_Head& prototype);
    virtual ~SpearMan_Head() = default;

public:
    static SpearMan_Head* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
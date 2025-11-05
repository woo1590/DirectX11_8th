#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class SpearMan_Spear
	:public PartObject
{
public:
    typedef struct tagSpearManSpearDesc : public PartObject::PART_OBJECT_DESC
    {

    }SPEAR_MAN_SPEAR_DESC;
private:
    SpearMan_Spear();
    SpearMan_Spear(const SpearMan_Spear& prototype);
    virtual ~SpearMan_Spear() = default;

public:
    static SpearMan_Spear* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
#pragma once
#include "UIObject.h"

NS_BEGIN(Client)

class SlotNum :
    public UIObject
{
public:
    typedef struct tagSlotNumDesc : public UIObject::UIOBJECT_DESC
    {
        _uint num{};
    }SLOT_NUM_DESC;
private:
    SlotNum();
    SlotNum(const SlotNum& prototype);
    virtual ~SlotNum() = default;

public:
    static SlotNum* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;
};

NS_END
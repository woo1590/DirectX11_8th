#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Door_L :
    public PartObject
{
public:
    typedef struct tagDoorLDesc : public PartObject::PART_OBJECT_DESC
    {

    }DOOR_L_DESC;
private:
    Door_L();
    Door_L(const Door_L& prototype);
    virtual ~Door_L() = default;

public:
    static Door_L* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
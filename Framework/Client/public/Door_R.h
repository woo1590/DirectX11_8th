#pragma once
#include "PartObject.h"

NS_BEGIN(Client)

class Door_R :
    public PartObject
{
public:
    typedef struct tagDoorRDesc : public PartObject::PART_OBJECT_DESC
    {

    }DOOR_R_DESC;
private:
    Door_R();
    Door_R(const Door_R& prototype);
    virtual ~Door_R() = default;

public:
    static Door_R* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
#pragma once
#include "ContainerObject.h"

NS_BEGIN(MapEditor)

class Door :
    public ContainerObject
{
public:
    typedef struct tagDoorDesc : ContainerObject::CONTAINER_OBJECT_DESC
    {

    }DOOR_DESC;
private:
    enum class Parts { Door_L, Door_R, Count };
    Door();
    Door(const Door& prototype);
    virtual ~Door() = default;

public:
    static Door* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
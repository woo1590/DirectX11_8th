#pragma once
#include "PartObject.h"

NS_BEGIN(MapEditor)

class Door_R :
    public PartObject
{
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
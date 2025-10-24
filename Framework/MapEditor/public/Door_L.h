#pragma once
#include "PartObject.h"

NS_BEGIN(MapEditor)

class Door_L :
    public PartObject
{
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
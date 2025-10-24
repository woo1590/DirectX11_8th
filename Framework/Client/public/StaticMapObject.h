#pragma once
#include "Object.h"

NS_BEGIN(Client)

class StaticMapObject :
    public Object
{
private:
    StaticMapObject();
    StaticMapObject(const StaticMapObject& prototype);
    virtual ~StaticMapObject() = default;

public:
    static StaticMapObject* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
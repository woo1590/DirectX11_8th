#pragma once
#include "Object.h"

NS_BEGIN(Client)

class StaticMapObject :
    public Object
{
private:
    StaticMapObject(_bool useEmissive);
    StaticMapObject(const StaticMapObject& prototype);
    virtual ~StaticMapObject() = default;

public:
    static StaticMapObject* Create(_bool useEmissive = false);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _bool m_UseEmissive = false;
};

NS_END
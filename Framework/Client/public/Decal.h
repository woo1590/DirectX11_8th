#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Decal :
    public Object
{
private:
    Decal();
    Decal(const Decal& prototype);
    virtual ~Decal() = default;

public:
    static Decal* Create();
};

NS_END
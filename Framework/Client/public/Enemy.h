#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class Enemy abstract:
    public ContainerObject
{
protected:
    Enemy();
    Enemy(const Enemy& prototype);
    virtual ~Enemy() = default;

public:
    virtual void Free()override;

};

NS_END
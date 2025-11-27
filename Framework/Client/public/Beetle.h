#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Beetle :
    public Enemy
{
private:
    Beetle();
    Beetle(const Beetle& prototype);
    virtual ~Beetle() = default;

public:
    static Beetle* Create();

};

NS_END
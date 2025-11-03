#pragma once
#include "Object.h"

NS_BEGIN(Client)

class PlayerInteractionComponent;
class Item :
    public Object
{
protected:
    Item();
    Item(const Item& prototype);
    virtual ~Item() = default;

public:
    virtual void Interaction(PlayerInteractionComponent* interaction) = 0;

protected:
};

NS_END
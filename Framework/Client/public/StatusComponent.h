#pragma once
#include "Component.h"

NS_BEGIN(Client)

class StatusComponent :
    public Component
{
public:
    typedef struct tagStatusDesc : public InitDESC
    {
        _float hp{};
        _float speed{};
        _float attackPower{};
    }STATUS_DESC;
private:
    StatusComponent(Object* owner);
    StatusComponent(const StatusComponent& prototype);
    virtual ~StatusComponent() = default;

public:
    static StatusComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

private:

};

NS_END
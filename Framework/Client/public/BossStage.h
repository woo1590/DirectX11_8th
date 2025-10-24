#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BossStage :
    public Object
{
private:
    BossStage();
    BossStage(const BossStage& prototype);
    virtual ~BossStage() = default;

public:
    static BossStage* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override {};
    void Update(_float dt)override {};
    void LateUpdate(_float dt)override {};

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
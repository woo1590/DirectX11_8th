#pragma once
#include "Enemy.h"

NS_BEGIN(Client)

class Boss :
    public Enemy
{
private:
    Boss();
    Boss(const Boss& prototype);
    virtual ~Boss()override;
    
public:
    static Boss* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
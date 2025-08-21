#pragma once
#include "Object.h"

NS_BEGIN(Client)

class BackGround final:
    public Object
{
private:
    BackGround();
    virtual ~BackGround() = default;

public:
    static BackGround* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(void* arg)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(void* arg)override;
    void Free()override;

private:

};

NS_END
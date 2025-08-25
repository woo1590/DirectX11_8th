#pragma once
#include "Object.h"

NS_BEGIN(Client)

class TestCube final:
    public Object
{
private:
    TestCube();
    virtual ~TestCube() = default;

public:
    static TestCube* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
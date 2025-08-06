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
    HRESULT Initialize()override;
    void Free()override;

    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

private:

};

NS_END
#pragma once

#include "PartObject.h"

NS_BEGIN(Client)

class TestObjectBody :
    public PartObject
{
private:
    TestObjectBody();
    TestObjectBody(const TestObjectBody& prototype);
    virtual ~TestObjectBody() = default;

public:
    static TestObjectBody* Create();
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
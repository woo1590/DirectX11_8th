#pragma once
#include "ContainerObject.h"

NS_BEGIN(Client)

class TestObject final:
    public ContainerObject
{
public:
    enum class Parts { Body, Count };
private:
    TestObject();
    virtual ~TestObject() = default;

public:
    static TestObject* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    HRESULT CreatePartObjects();

};

NS_END
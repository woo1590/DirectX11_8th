#pragma once
#include "Level.h"

NS_BEGIN(Client)

class TestLevel final:
    public Level
{
private:
    TestLevel();
    virtual ~TestLevel() = default;

public:
    static TestLevel* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    HRESULT InitLayerBackGround(const _string& layerTag);
    HRESULT InitLayerTest(const _string& layerTag);

};

NS_END
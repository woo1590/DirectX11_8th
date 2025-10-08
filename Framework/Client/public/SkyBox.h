#pragma once
#include "Object.h"

NS_BEGIN(Client)

class SkyBox :
    public Object
{
private:
    SkyBox();
    SkyBox(const SkyBox& prototype);
    virtual ~SkyBox() = default;

public:
    static SkyBox* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:

};

NS_END
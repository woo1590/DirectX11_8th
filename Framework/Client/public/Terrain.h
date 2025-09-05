#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Terrain :
    public Object
{
private:
    Terrain();
    Terrain(const Terrain& prototype);
    virtual ~Terrain() = default;

public:
    static Terrain* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override; 
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    void Free()override;
    Object* Clone(InitDESC* arg)override;

private:
    VIBuffer* m_pVIBuffer = nullptr;
    Material* m_pMaterial = nullptr;
};

NS_END
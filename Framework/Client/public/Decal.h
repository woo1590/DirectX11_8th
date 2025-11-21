#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Decal :
    public Object
{
public:
    typedef struct tagDecalDesc : public Object::OBJECT_DESC
    {
        _float3 surfaceDir{};
    }DECAL_DESC;
private:
    Decal();
    Decal(const Decal& prototype);
    virtual ~Decal() = default;

public:
    static Decal* Create();
    HRESULT Initialize_Prototype();
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float m_fElapsedTime{};
    _float m_fDuration = 2.f;

};

NS_END
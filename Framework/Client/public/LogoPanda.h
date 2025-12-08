#pragma once
#include "Object.h"

NS_BEGIN(Engine)
class ModelComponent;
NS_END

NS_BEGIN(Client)

class LogoPanda :
    public Object
{
    LogoPanda();
    LogoPanda(const LogoPanda& prototype);
    virtual ~LogoPanda() = default;

public:
    static LogoPanda* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    ModelComponent* m_pOutlineModel = nullptr;
};

NS_END
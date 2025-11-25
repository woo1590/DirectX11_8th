#pragma once
#include "Object.h"

NS_BEGIN(Client)

class Torch :
    public Object
{
public:
    typedef struct tagTorchDesc : public Object::OBJECT_DESC
    {
        _float range{};
        _float4 color{};
    }TORCH_DESC;
private:
    Torch();
    Torch(const Torch& prototype);
    virtual ~Torch() = default;

public:
    static Torch* Create();
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;
    HRESULT ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    Object* m_pTorchFireEffect = nullptr;
};

NS_END
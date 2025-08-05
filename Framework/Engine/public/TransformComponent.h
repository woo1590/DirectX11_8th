#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL TransformComponent final:
    public Component
{
private:
    TransformComponent(Object* owner);
    virtual ~TransformComponent() = default;

public:
    static TransformComponent* Create(Object* owner);
    HRESULT Initialize();
    void Free()override;

private:
    _float3 position{};
    _float3 scale{};
    _float3 rotation{};
};

NS_END
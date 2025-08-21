#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL ModelComponent final:
    public Component
{
private:
    ModelComponent(Object* owner);
    virtual ~ModelComponent() = default;

public:
    static ModelComponent* Create(Object* owner);
    HRESULT Initialize();
    void Free()override;
    
private:

};

NS_END
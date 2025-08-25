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
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Free()override;
    
    Component* Clone(InitDESC* arg) { return nullptr; }

private:

};

NS_END
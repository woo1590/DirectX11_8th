#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL ModelComponent final:
    public Component
{
private:
    ModelComponent(Object* owner);
    ModelComponent(const ModelComponent& prototype);
    virtual ~ModelComponent() = default;

public:
    static ModelComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Free()override;
    
    Component* Clone()override;

private:

};

NS_END
#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class Model;
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
    
    HRESULT Bind();

    Component* Clone()override;
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif
private:
    Model* m_pModel = nullptr;
};

NS_END
#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class TransformComponent;
class Model;
NS_END

NS_BEGIN(Importer)

class ModelLoderComponent :
    public Component
{
private:
    ModelLoderComponent(Object* owner);
    ModelLoderComponent(const ModelLoderComponent& prototype);
    virtual ~ModelLoderComponent() = default;

public:
    static ModelLoderComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;
    HRESULT ExtractRenderProxies(TransformComponent* transform, std::vector<RenderProxy>& proxies);

    HRESULT ImportModel(const _string& filePath);

    Component* Clone() { return new ModelLoderComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
    void ImportInspector(_string openedFile);

#endif
};

NS_END
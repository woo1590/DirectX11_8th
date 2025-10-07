#pragma once
#include "Component.h"

NS_BEGIN(MapEditor)

class MakePrefabComponent :
    public Component
{
public:
    typedef struct tagMakePrefabComponentDesc :public InitDESC
    {
        _string prototypeTag{};
        _string layerTag{};
        _string modelTag{};
    }MAKE_PREFAB_DESC;
private:
    MakePrefabComponent(Object* owner);
    MakePrefabComponent(const MakePrefabComponent& prototype);
    virtual ~MakePrefabComponent() = default;

public:
    static MakePrefabComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    PREFAB ExportPrefab();

    Component* Clone()override { return new MakePrefabComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {}
#endif

private:
    _string m_strPrototypeTag{};
    _string m_strModelTag{};
    _string m_strLayerTag{};
};

NS_END
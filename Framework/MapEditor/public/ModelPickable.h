#pragma once
#include "PickableComponent.h"

NS_BEGIN(Engine)
class Model;
NS_END

NS_BEGIN(MapEditor)

class ModelPickable :
    public PickableComponent
{
private:
    ModelPickable(Object* owner);
    ModelPickable(const ModelPickable& prototype);
    virtual ~ModelPickable() = default;

public:
    static ModelPickable* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    PICK_RESULT IntersectRay(RAY ray)override;
    void SetModel(_uint levelID, const _string& key);

    Component* Clone()override { return new ModelPickable(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif 

private:
    Model* m_pModel = nullptr;

};

NS_END
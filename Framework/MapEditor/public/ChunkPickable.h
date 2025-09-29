#pragma once
#include "PickableComponent.h"

NS_BEGIN(MapEditor)

class ChunkPickable :
    public PickableComponent
{
private:
    ChunkPickable(Object* owner);
    ChunkPickable(const ChunkPickable& prototype);
    virtual ~ChunkPickable() = default;

public:
    static ChunkPickable* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Update(_float dt)override;

    PICK_RESULT IntersectRay(RAY ray);

    Component* Clone()override { return new ChunkPickable(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector() {};
#endif

private:
    _float4 m_PlaneXZ{};
};

NS_END
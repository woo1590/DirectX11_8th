#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class ENGINE_DLL Bounding_OBB :
    public Bounding
{
public:
    typedef struct tagBoundingObbDesc : public BOUNDING_DESC
    {
        _float3 halfSize{};
        _float3 rotation{};
    }OBB_DESC;
private:
    Bounding_OBB();
    virtual ~Bounding_OBB();

public:
    static Bounding_OBB* Create(BOUNDING_DESC* arg);
    HRESULT Initialize(BOUNDING_DESC* arg);
    
    void Update(_float4x4 worldMatrix)override;
    _bool Intersect(Bounding* other)override;
    RAYCAST_DATA RayCast(RAY worldRay)override;
    _bool IntersectToAABB(Bounding_AABB* other) override;
    _bool IntersectToOBB(Bounding_OBB* other) override;
    _bool IntersectToSphere(Bounding_Sphere* other) override;

    BoundingOrientedBox GetWorldDesc()const { return m_WorldDesc; }

    void Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color) override;

    void Free()override;

#ifdef USE_IMGUI
    virtual void RenderInspector()override;
#endif
private:
    BoundingOrientedBox m_LocalDesc{};
    BoundingOrientedBox m_WorldDesc{};



};

NS_END
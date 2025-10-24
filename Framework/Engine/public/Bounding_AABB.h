#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class ENGINE_DLL Bounding_AABB final:
    public Bounding
{
public:
    typedef struct tagBoudingAABBDesc : Bounding::BOUNDING_DESC
    {
        _float3 halfSize{ 1.f,1.f,1.f };
    }AABB_DESC;
private:
    Bounding_AABB();
    virtual ~Bounding_AABB() = default;


public:
    static Bounding_AABB* Create(BOUNDING_DESC* arg);
    HRESULT Initialize(BOUNDING_DESC* arg);

    void Update(_float4x4 worldMatrix)override;
    _bool Intersect(Bounding* other)override;
    _bool Intersect(RAY worldRay, _float& distance)override;
    _bool IntersectToAABB(Bounding_AABB* other) override;
    _bool IntersectToOBB(Bounding_OBB* other) override;
    _bool IntersectToSphere(Bounding_Sphere* other) override;

    BoundingBox GetWorldDesc()const { return m_WorldDesc; }

    void Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color)override;

    void Free()override;

#ifdef USE_IMGUI
    virtual void RenderInspector()override;
#endif
private:
    BoundingBox m_LocalDesc{};
    BoundingBox m_WorldDesc{};
};

NS_END
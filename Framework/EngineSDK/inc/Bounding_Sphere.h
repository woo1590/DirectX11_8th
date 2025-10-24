#pragma once
#include "Bounding.h"

NS_BEGIN(Engine)

class ENGINE_DLL Bounding_Sphere :
    public Bounding
{
public:
    typedef struct tagBoundingSphereDesc : public BOUNDING_DESC
    {
        _float radius = 1.f;

    }SPHERE_DESC;
private:
    Bounding_Sphere();
    virtual ~Bounding_Sphere() = default;

public:
    static Bounding_Sphere* Create(BOUNDING_DESC* arg);
    HRESULT Initialize(BOUNDING_DESC* arg);

    void Update(_float4x4 worldMatrix) override;
    _bool Intersect(Bounding* other)override;
    _bool Intersect(RAY worldRay, _float& distance)override { return false; }
    _bool IntersectToAABB(Bounding_AABB* other) override;
    _bool IntersectToOBB(Bounding_OBB* other) override;
    _bool IntersectToSphere(Bounding_Sphere* other) override;

    BoundingSphere GetWorldDesc()const { return m_WorldDesc; }

    void Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color) override;
    void Free()override;

#ifdef USE_IMGUI
    virtual void RenderInspector()override;
#endif

private:
    BoundingSphere m_LocalDesc{};
    BoundingSphere m_WorldDesc{};

};

NS_END  
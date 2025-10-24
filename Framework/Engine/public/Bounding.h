#pragma once
#include "Base.h"
#include "DebugDraw.h"

NS_BEGIN(Engine)

class Bounding_AABB;
class Bounding_OBB;
class Bounding_Sphere;
class ENGINE_DLL Bounding abstract:
    public Base
{
public:
    typedef struct tagBoudingDesc : public InitDESC
    {
        ColliderType type = ColliderType::Count;
        _float3  center{};
    }BOUNDING_DESC;

protected:
    Bounding();
    virtual ~Bounding() = default;

public:
    virtual void Update(_float4x4 worldMatrix) = 0;
    virtual _bool Instersect(Bounding* other) = 0;
    virtual _bool IntersectToAABB(Bounding_AABB* other) = 0;
    virtual _bool IntersectToOBB(Bounding_OBB* other) = 0;
    virtual _bool IntersectToSphere(Bounding_Sphere* other) = 0;

    virtual void Draw(PrimitiveBatch<VertexPositionColor>* batch, _float4 color) = 0;
    virtual void Free()override;

#ifdef USE_IMGUI
    virtual void RenderInspector() = 0;
#endif

protected:

    
};

NS_END
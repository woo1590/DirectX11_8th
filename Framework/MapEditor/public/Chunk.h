#pragma once
#include "Object.h"

NS_BEGIN(MapEditor)

typedef struct tagCell
{
    _float3 position{};
}CELL;

typedef struct tagCBPerChunk
{
    _float3 chunkMin{};
    _float cellSize{};
    _float3 hoverPosition{ -317.5f,0.f,-317.5f };
    _float pad{};

}CB_PER_CHUNK;

class PickingSystem;
class Chunk :
    public Object
{
public:
    typedef struct tagChunkDesc :public Object::OBJECT_DESC
    {
        _float3 chunkPosition{};
    }CHUNK_DESC;
private:
    Chunk();
    Chunk(const Chunk& prototype);
    virtual ~Chunk() = default;

public:
    static Chunk * Create(PickingSystem* picking);
    HRESULT Initialize_Prototype(PickingSystem* picking);
    HRESULT Initialize(InitDESC* arg)override;
    void PriorityUpdate(_float dt)override;
    void Update(_float dt)override;
    void LateUpdate(_float dt)override;

    Object* Clone(InitDESC* arg)override;
    void Free()override;

private:
    _float3 GetHoverPosition();
    void UpdatePlane();
    void UpdateConstantBuffer();
    HRESULT BindConstantBuffer();

    CELL m_Cells[CHUNK_SIZE][CHUNK_SIZE];

    _float4 m_PlaneXZ{};
    ID3D11Buffer* m_pCBPerChunk = nullptr;

    PickingSystem* m_pPickingSystem = nullptr;
};

NS_END
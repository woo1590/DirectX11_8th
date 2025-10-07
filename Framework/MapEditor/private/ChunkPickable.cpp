#include "MapEditorPCH.h"
#include "ChunkPickable.h"
#include "Object.h"
#include "TransformComponent.h"

ChunkPickable::ChunkPickable(Object* owner)
	:PickableComponent(owner)
{
}

ChunkPickable::ChunkPickable(const ChunkPickable& prototype)
	:PickableComponent(prototype)
{
}

ChunkPickable* ChunkPickable::Create(Object* owner)
{
	ChunkPickable* Instance = new ChunkPickable(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);
	
	return Instance;
}

HRESULT ChunkPickable::Initialize_Prototype()
{
	return S_OK;
}

HRESULT ChunkPickable::Initialize(InitDESC* arg)
{
	return S_OK;
}

void ChunkPickable::Update(_float dt)
{
	_float y = m_pOwner->GetComponent<TransformComponent>()->GetPosition().y;

	m_PlaneXZ = _float4{ 0.f,1.f,0.f,-y };
}

PICK_RESULT ChunkPickable::IntersectRay(RAY ray)
{
	_float3 chunkPosition = m_pOwner->GetComponent<TransformComponent>()->GetPosition();
	_float3 chunkMinXZ;
	chunkMinXZ.x = chunkPosition.x - (CELL_SIZE * CHUNK_SIZE * 0.5f);
	chunkMinXZ.y = 0.f;
	chunkMinXZ.z = chunkPosition.z - (CELL_SIZE * CHUNK_SIZE * 0.5f);

	_vector posA = XMLoadFloat3(&ray.origin);
	_vector posB = posA + XMLoadFloat3(&ray.direction);
	_float3 hitPosition;
	XMStoreFloat3(&hitPosition, XMPlaneIntersectLine(XMLoadFloat4(&m_PlaneXZ), posA, posB));

	PICK_RESULT result{};
	result.type = PickType::Chunk;
	result.isHit = true;
	result.indexX = std::floor((hitPosition.x - chunkMinXZ.x) / CELL_SIZE);
	result.indexZ = std::floor((hitPosition.z - chunkMinXZ.z) / CELL_SIZE);
	result.chunkPosition = chunkPosition;
	XMStoreFloat(&result.distance, XMVector3Length(XMLoadFloat3(&hitPosition) - XMLoadFloat3(&ray.origin)));

	return result;
}

void ChunkPickable::Free()
{
	__super::Free();
}

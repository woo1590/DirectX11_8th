#include "EnginePCH.h"
#include "MeshColliderComponent.h"
#include "EngineCore.h"
#include "Model.h"
#include "Object.h"
#include "TransformComponent.h"

MeshColliderComponent::MeshColliderComponent(Object* owner)
	:ColliderComponent(owner)
{
}

MeshColliderComponent::MeshColliderComponent(const MeshColliderComponent& prototype)
	:ColliderComponent(prototype)
{
}

MeshColliderComponent* MeshColliderComponent::Create(Object* owner)
{
	MeshColliderComponent* Instance = new MeshColliderComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MeshColliderComponent::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT MeshColliderComponent::Initialize(InitDESC* arg)
{
	MESH_COLLIDER_DESC* desc = static_cast<MESH_COLLIDER_DESC*>(arg);
	m_iColliderFilter = desc->colliderFilter;

	return S_OK;
}

RAYCAST_DATA MeshColliderComponent::RayCast(RAY worldRay)
{
	RAYCAST_DATA result{};
	_float4x4 worldInv = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrixInverse();
	_float4x4 world = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrix();

	RAY localRay{};

	XMStoreFloat3(&localRay.origin, XMVector3TransformCoord(XMLoadFloat3(&worldRay.origin), XMLoadFloat4x4(&worldInv)));
	XMStoreFloat3(&localRay.direction, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&worldRay.direction), XMLoadFloat4x4(&worldInv))));

	RAY_HIT_DATA data{};
	data = m_pModel->RayCastLocal(localRay, PickingType::Triangle);

	if (data.isHit)
	{
		result.isHit = true;
		result.hitObject = m_pOwner;

		_float3 localHitPosition{};
		XMStoreFloat3(&localHitPosition, XMVectorMultiplyAdd(XMLoadFloat3(&localRay.direction), XMVectorReplicate(data.localDistance), XMLoadFloat3(&localRay.origin)));

		_float3 worldHitPosition{};
		XMStoreFloat3(&worldHitPosition, XMVector3TransformCoord(XMLoadFloat3(&localHitPosition), XMLoadFloat4x4(&world)));
		XMStoreFloat(&result.worldDistance, XMVector3Length(XMLoadFloat3(&worldHitPosition) - XMLoadFloat3(&worldRay.origin)));

		XMStoreFloat3(&result.hitNormal, XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&data.normal), XMLoadFloat4x4(&world))));
	}

	return result;
}

HRESULT MeshColliderComponent::SetModel(_uint levelID, const _string& modelTag)
{
	auto model = EngineCore::GetInstance()->GetModel(levelID, modelTag);
	if (!model)
		return E_FAIL;

	m_pModel = model;
	m_pModel->AddRef();

	return S_OK;
}

void MeshColliderComponent::Free()
{
	__super::Free();

	Safe_Release(m_pModel);
}

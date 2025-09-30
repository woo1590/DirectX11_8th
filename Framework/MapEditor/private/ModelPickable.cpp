#include "MapEditorPCH.h"
#include "ModelPickable.h"
#include "Object.h"	
#include "TransformComponent.h"
#include "EngineCore.h"
#include "Model.h"

ModelPickable::ModelPickable(Object* owner)
	:PickableComponent(owner)
{
}

ModelPickable::ModelPickable(const ModelPickable& prototype)
	:PickableComponent(prototype)
{
}

ModelPickable* ModelPickable::Create(Object* owner)
{
	ModelPickable* Instance = new ModelPickable(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ModelPickable::Initialize_Prototype()
{
	return S_OK;
}

HRESULT ModelPickable::Initialize(InitDESC* arg)
{
	return S_OK;
}

PICK_RESULT ModelPickable::IntersectRay(RAY worldRay)
{
	RAY localRay{};
	RAY_HIT_DATA hitData{};
	PICK_RESULT result{};

	_float4x4 worldInv = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrixInverse();
	_float4x4 worldMat = m_pOwner->GetComponent<TransformComponent>()->GetWorldMatrix();
	XMStoreFloat3(&localRay.origin, XMVector3TransformCoord(XMLoadFloat3(&worldRay.origin), XMLoadFloat4x4(&worldInv)));
	XMStoreFloat3(&localRay.direction, XMVector3TransformNormal(XMLoadFloat3(&worldRay.direction), XMLoadFloat4x4(&worldInv)));

	hitData = m_pModel->RayCastLocal(localRay, PickingType::Triangle);

	result.isHit = hitData.isHit;
	if (result.isHit)
	{
		_float3 localHitPosition{};
		XMStoreFloat3(&localHitPosition, XMVectorMultiplyAdd(XMLoadFloat3(&localRay.direction), XMVectorReplicate(hitData.localDistance), XMLoadFloat3(&localRay.origin)));

		_float3 worldHitPosition{};
		XMStoreFloat3(&worldHitPosition, XMVector3TransformCoord(XMLoadFloat3(&localHitPosition), XMLoadFloat4x4(&worldMat)));
		XMStoreFloat(&result.distance, XMVector3Length(XMLoadFloat3(&worldHitPosition) - XMLoadFloat3(&worldRay.origin)));

		result.hitPosition = worldHitPosition;
		result.object = m_pOwner;
		result.type = PickType::Model;
	}

	return result;
}

void ModelPickable::SetModel(_uint levelID, const _string& key)
{
	m_pModel = EngineCore::GetInstance()->GetModel(levelID, key);
	m_pModel->AddRef();
}

void ModelPickable::Free()
{
	__super::Free();

	Safe_Release(m_pModel);
}

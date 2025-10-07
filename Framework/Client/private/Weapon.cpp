#include "pch.h"
#include "Weapon.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Weapon::Weapon()
	:PartObject()
{
}

Weapon::Weapon(const Weapon& prototype)
	:PartObject(prototype)
{
}

HRESULT Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Weapon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	m_iBip001_R_Hand_Index = model->GetBoneIndex("Bip001 R Hand");

	return S_OK;
}

void Weapon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Weapon::Update(_float dt)
{
	__super::Update(dt);

	_float4x4 combined = GetComponent<AnimatorComponent>()->GetCombinedMatrices()[m_iBip001_R_Hand_Index];

	/*Inverse Matrix*/
	XMStoreFloat4x4(&combined, XMMatrixInverse(nullptr, XMLoadFloat4x4(&combined)));

	/*Decompose matrix*/
	_vector positionV, scaleV, quaternionV;
	_float3 position, scale;
	_float4 quaternion;
	XMMatrixDecompose(&scaleV, &quaternionV, &positionV, XMLoadFloat4x4(&combined));

	/*set position*/
	XMStoreFloat3(&position, positionV);
	m_pTransform->SetPosition(position);

	/*set scale*/
	XMStoreFloat3(&scale, scaleV);
	m_pTransform->SetScale(scale);

	/*set quaternion*/
	XMStoreFloat4(&quaternion, XMQuaternionNormalize(quaternionV));
	m_pTransform->SetQuaternion(quaternion);
	
}

void Weapon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Weapon::Free()
{
	__super::Free();
}

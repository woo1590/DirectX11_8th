#include "pch.h"
#include "PlayerAnimController.h"

#include "AnimationClip.h"

//object
#include "Object.h"

//component
#include "AnimatorComponent.h"
#include "ModelComponent.h"

PlayerAnimController::PlayerAnimController(Object* owner)
	:Component(owner)
{
}

PlayerAnimController::PlayerAnimController(const PlayerAnimController& prototype)
	:Component(prototype)
{
}

PlayerAnimController* PlayerAnimController::Create(Object* owner)
{
	PlayerAnimController* Instance = new PlayerAnimController(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PlayerAnimController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT PlayerAnimController::Initialize(InitDESC* arg)
{
	return S_OK;
}

void PlayerAnimController::Update(_float dt)
{
	if (!m_pHandAnimator || !m_pWeaponAnimator)
		return;

	m_HandMasks.assign(m_iNumHandBones, 0);
	auto& weaponTransformations = m_pWeaponAnimator->GetTransformationMatrices();

	for (_uint i = 0; i < weaponTransformations.size(); ++i)
	{
		_int index = m_BoneMap.mappedBoneIndex[i];
		if (-1 == index || 0 == index)
			continue;

		m_HandOverrideMatrices[index] = weaponTransformations[i];
		m_HandMasks[index] = 1;
	}

	m_pHandAnimator->SetOverride(m_HandOverrideMatrices, m_HandMasks);
}

void PlayerAnimController::SetWeaponAnimator(AnimatorComponent* weaponAnimator)
{
	Safe_Release(m_pWeaponAnimator);

	m_pWeaponAnimator = weaponAnimator;
	m_pWeaponAnimator->AddRef();

	m_BoneMap = m_pHandAnimator->MakeBoneMap(m_pWeaponAnimator);
}

void PlayerAnimController::SetHandAnimator(AnimatorComponent* handAnimator)
{
	m_pHandAnimator = handAnimator;
	m_pHandAnimator->AddRef();

	m_iNumHandBones = m_pHandAnimator->GetNumBones();
	m_HandOverrideMatrices.resize(m_iNumHandBones);
	m_HandMasks.resize(m_iNumHandBones);
}

void PlayerAnimController::SetBoneMap(BONE_MAP boneMap)
{
	m_BoneMap = boneMap;
}

void PlayerAnimController::Free()
{
	__super::Free();

	Safe_Release(m_pHandAnimator);
	Safe_Release(m_pWeaponAnimator);
}

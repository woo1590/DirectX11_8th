#include "EnginePCH.h"
#include "AnimatorComponent.h"
#include "EngineCore.h"
#include "AnimationClip.h"

AnimatorComponent::AnimatorComponent(Object* owner)
	:Component(owner)
{
}

AnimatorComponent::AnimatorComponent(const AnimatorComponent& prototype)
	:Component(prototype)
{
}

AnimatorComponent* AnimatorComponent::Create(Object* owner)
{
	AnimatorComponent* Instance = new AnimatorComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT AnimatorComponent::Initialize_Prototype()
{

	return S_OK;
}

HRESULT AnimatorComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void AnimatorComponent::Update(_float dt)
{
	__super::Update(dt);

	if (m_iCurrAnimationIndex >= m_AnimationSet.numAnimations)
		return;


}

HRESULT AnimatorComponent::SetAnimation(_uint levelID, const _string& key)
{
	m_AnimationSet = EngineCore::GetInstance()->GetAnimation(levelID, key);

	if (!m_AnimationSet.numAnimations)
	{
		MSG_BOX("Empty animation");
		return E_FAIL;
	}

	for (auto& animation : m_AnimationSet.aniamtionClips)
		animation->AddRef();

	return S_OK;
}

void AnimatorComponent::Free()
{
	__super::Free();
}

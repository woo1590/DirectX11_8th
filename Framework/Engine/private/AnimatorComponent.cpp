#include "EnginePCH.h"
#include "AnimatorComponent.h"

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

void AnimatorComponent::Free()
{
	__super::Free();
}

#include "pch.h"
#include "EffectContainer.h"

EffectContainer::EffectContainer()
	:ContainerObject()
{
}

EffectContainer::EffectContainer(const EffectContainer& prototype)
	:ContainerObject(prototype)
{
}

EffectContainer* EffectContainer::Create()
{
	EffectContainer* Instance = new EffectContainer();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectContainer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT EffectContainer::Initialize(InitDESC* arg)
{
	EFFECT_CONTAINER_DESC* desc = static_cast<EFFECT_CONTAINER_DESC*>(arg);

	if (FAILED(__super::Initialize(desc)))
		return E_FAIL;

	return S_OK;
}

void EffectContainer::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectContainer::Update(_float dt)
{
	__super::Update(dt);
}

void EffectContainer::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectContainer::Clone(InitDESC* arg)
{
	EffectContainer* Instance = new EffectContainer(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectContainer::Free()
{
	__super::Free();
}

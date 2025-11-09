#include "pch.h"
#include "DamageFont.h"

//component
#include "SpriteComponent.h"

DamageFont::DamageFont()
	:Object()
{
}

DamageFont::DamageFont(const DamageFont& prototype)
	:Object(prototype)
{
}

DamageFont* DamageFont::Create()
{
	DamageFont* Instance = new DamageFont();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DamageFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT DamageFont::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void DamageFont::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DamageFont::Update(_float dt)
{
	__super::Update(dt);
}

void DamageFont::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* DamageFont::Clone(InitDESC* arg)
{
	DamageFont* Instance = new DamageFont(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DamageFont::Free()
{
	__super::Free();
}

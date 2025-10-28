#include "pch.h"
#include "SpearMan.h"

SpearMan::SpearMan()
	:Enemy()
{
}

SpearMan::SpearMan(const SpearMan& prototype)
	:Enemy(prototype)
{
}

SpearMan* SpearMan::Create()
{
	SpearMan* Instance = new SpearMan();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;	
}

HRESULT SpearMan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT SpearMan::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void SpearMan::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SpearMan::Update(_float dt)
{
	__super::Update(dt);
}

void SpearMan::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* SpearMan::Clone(InitDESC* arg)
{
	SpearMan* Instance = new SpearMan(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SpearMan::Free()
{
	__super::Free();
}

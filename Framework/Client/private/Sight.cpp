#include "pch.h"
#include "Sight.h"

Sight::Sight()
	:UIObject()
{
}

Sight::Sight(const Sight& prototype)
	:UIObject(prototype)
{
}

Sight* Sight::Create()
{
	Sight* Instance = new Sight();

	if(FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Sight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Sight::Initialize(InitDESC* arg)
{
	return E_NOTIMPL;
}

void Sight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Sight::Update(_float dt)
{
	__super::Update(dt);
}

void Sight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Sight::Clone(InitDESC* arg)
{
	Sight* Instance = new Sight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Sight::Free()
{
	__super::Free();
}

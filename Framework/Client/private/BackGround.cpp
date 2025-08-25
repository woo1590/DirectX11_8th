#include "pch.h"
#include "BackGround.h"

BackGround::BackGround()
{
}

BackGround* BackGround::Create()
{
	BackGround* Instance = new BackGround();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BackGround::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT BackGround::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	return S_OK;
}

void BackGround::Update(_float dt)
{
	__super::Update(dt);
}

void BackGround::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BackGround::Clone(InitDESC* arg)
{
	BackGround* Instance = new BackGround(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BackGround::Free()
{
	__super::Free();
}

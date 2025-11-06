#include "pch.h"
#include "Sun.h"

//component	
#include "LightComponent.h"

Sun::Sun()
	:Object()
{
}

Sun::Sun(const Sun& prototype)
	:Object(prototype)
{
}

Sun* Sun::Create()
{
	Sun* Instance = new Sun();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Sun::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT Sun::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.type = LightType::Directional;
	lightDesc.color = _float4(0.4f, 0.4f, 0.4f, 1.f);
	lightDesc.direction = _float4(1.f, -1.f, 1.f, 0.f);

	if (FAILED(GetComponent<LightComponent>()->Initialize(&lightDesc)))
		return E_FAIL;

	return S_OK;
}

void Sun::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Sun::Update(_float dt)
{
	__super::Update(dt);
}

void Sun::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Sun::Clone(InitDESC* arg)
{
	Sun* Instance = new Sun(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Sun::Free()
{
	__super::Free();
}

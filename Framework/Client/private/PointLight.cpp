#include "pch.h"
#include "PointLight.h"

//component
#include "LightComponent.h"

PointLight::PointLight()
	:Object()
{
}

PointLight::PointLight(const PointLight& prototype)
	:Object(prototype)
{
}

PointLight* PointLight::Create()
{
	PointLight* Instance = new PointLight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PointLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT PointLight::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	POINT_LIGHT_DESC* desc = static_cast<POINT_LIGHT_DESC*>(arg);

	/*light*/
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.type = LightType::Point;
	lightDesc.color = desc->color;
	lightDesc.range = desc->range;

	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);

	engine->RegisterLight(light);

	return S_OK;
}

void PointLight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PointLight::Update(_float dt)
{
	__super::Update(dt);
}

void PointLight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* PointLight::Clone(InitDESC* arg)
{
	PointLight* Instance = new PointLight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PointLight::Free()
{
	EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
}

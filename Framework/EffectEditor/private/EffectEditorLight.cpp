#include "pch.h"
#include "EffectEditorLight.h"

//component
#include "LightComponent.h"

EffectEditorLight::EffectEditorLight()
	:Object()
{
}

EffectEditorLight::EffectEditorLight(const EffectEditorLight& prototype)
	:Object(prototype)
{
}

EffectEditorLight* EffectEditorLight::Create()
{
	EffectEditorLight* Instance = new EffectEditorLight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectEditorLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EffectEditorLight";

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT EffectEditorLight::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	LightComponent::LIGHT_DESC desc{};
	desc.color = _float4(1.f, 1.f, 1.f, 1.f);
	desc.direction = _float4(1.f, -1.f, 1.f, 0.f);
	desc.type = LightType::Directional;

	if (FAILED(GetComponent<LightComponent>()->Initialize(&desc)))
		return E_FAIL;

	return S_OK;
}

void EffectEditorLight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectEditorLight::Update(_float dt)
{
	__super::Update(dt);
}

void EffectEditorLight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* EffectEditorLight::Clone(InitDESC* arg)
{
	EffectEditorLight* Instance = new EffectEditorLight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectEditorLight::Free()
{
	__super::Free();
}

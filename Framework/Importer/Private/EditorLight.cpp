#include "ImporterPCH.h"
#include "EditorLight.h"

//component
#include "LightComponent.h"

EditorLight::EditorLight()
	:Object()
{
}

EditorLight::EditorLight(const EditorLight& prototype)
	:Object(prototype)
{
}

EditorLight* EditorLight::Create()
{
	EditorLight* Instance = new EditorLight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EditorLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "EditorLight";

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT EditorLight::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.direction = _float4(1.f, -1.f, 1.f, 0.f);
	lightDesc.color = _float4(1.f, 1.f, 1.f, 1.f);
	lightDesc.type = LightType::Directional;

	if (FAILED(GetComponent<LightComponent>()->Initialize(&lightDesc)))
		return E_FAIL;

	return S_OK;
}

Object* EditorLight::Clone(InitDESC* arg)
{
	EditorLight* Instance = new EditorLight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EditorLight::Free()
{
	__super::Free();
}

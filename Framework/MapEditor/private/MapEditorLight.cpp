#include "MapEditorPCH.h"
#include "MapEditorLight.h"

//component
#include "LightComponent.h"

MapEditorLight::MapEditorLight()
	:Object()
{
}

MapEditorLight::MapEditorLight(const MapEditorLight& prototype)
	:Object(prototype)
{
}

MapEditorLight* MapEditorLight::Create()
{
	MapEditorLight* Instance = new MapEditorLight();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT MapEditorLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "MapEditorLight";

	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT MapEditorLight::Initialize(InitDESC* arg)
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

void MapEditorLight::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MapEditorLight::Update(_float dt)
{
	__super::Update(dt);
}

void MapEditorLight::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* MapEditorLight::Clone(InitDESC* arg)
{
	MapEditorLight* Instance = new MapEditorLight(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MapEditorLight::Free()
{
	__super::Free();
}

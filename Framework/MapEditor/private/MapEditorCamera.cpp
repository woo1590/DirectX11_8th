#include "MapEditorPCH.h"
#include "MapEditorCamera.h"

//component
#include "CameraComponent.h"

MapEditorCamera::MapEditorCamera()
	:Object()
{
}

MapEditorCamera::MapEditorCamera(const MapEditorCamera& prototype)
	:Object(prototype)
{
}

MapEditorCamera* MapEditorCamera::Create()
{
	MapEditorCamera* Instance = new MapEditorCamera();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);


	return Instance;
}

HRESULT MapEditorCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "MapEditorCamera";

	AddComponent<CameraComponent>();

	return S_OK;
}

HRESULT MapEditorCamera::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CameraComponent::CameraDESC camDesc{};
	camDesc.aspect = static_cast<_float>(WinSizeX) / WinSizeY;
	camDesc.fov = XMConvertToRadians(60.f);
	camDesc.nearZ = 1.f;
	camDesc.farZ = 1000.f;

	if (FAILED(GetComponent<CameraComponent>()->Initialize(&camDesc)))
		return E_FAIL;

	return S_OK;
}

void MapEditorCamera::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void MapEditorCamera::Update(_float dt)
{
	__super::Update(dt);
}

void MapEditorCamera::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* MapEditorCamera::Clone(InitDESC* arg)
{
	MapEditorCamera* Instance = new MapEditorCamera(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void MapEditorCamera::Free()
{
	__super::Free();
}

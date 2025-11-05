#include "EnginePCH.h"
#include "CameraManager.h"
#include "EngineCore.h"

//component
#include "CameraComponent.h"

CameraManager::CameraManager()
{
}

CameraManager* CameraManager::Create()
{
	CameraManager* Instance = new CameraManager();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CameraManager::Initialize()
{
	return S_OK;
}

void CameraManager::Update()
{
	/*Update inverse matrix & cam position*/
	if (!m_MainCamera)
		return;

	m_MainCameraContext.viewMatrix = m_MainCamera->GetViewMatrix();
	m_MainCameraContext.projMatrix = m_MainCamera->GetProjMatrix();

	XMStoreFloat4x4(&m_MainCameraContext.viewMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MainCameraContext.viewMatrix)));

	XMStoreFloat4x4(&m_MainCameraContext.projMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MainCameraContext.projMatrix)));

	_float4x4 viewMatrixInverse = m_MainCameraContext.viewMatrixInverse;
	m_MainCameraContext.camPosition = _float3{ viewMatrixInverse._41,viewMatrixInverse._42,viewMatrixInverse._43 };

	m_MainCameraContext.farZ = m_MainCamera->GetFarZ();
	m_MainCameraContext.nearZ = m_MainCamera->GetNearZ();
}

void CameraManager::SetMainCamera(const _string& cameraTag)
{
	auto iter = m_Cameras.find(cameraTag);

	if (iter == m_Cameras.end())
		MSG_BOX("Invalid camera tag");

	Safe_Release(m_MainCamera);

	m_MainCamera = iter->second;
	m_MainCamera->AddRef();
}

void CameraManager::AddCamera(const _string& cameraTag, CameraComponent* component)
{
	m_Cameras.emplace(cameraTag, component);
	component->AddRef();
}

void CameraManager::Free()
{
	__super::Free();

	for (auto& pair : m_Cameras)
		Safe_Release(pair.second);

	Safe_Release(m_MainCamera);
}

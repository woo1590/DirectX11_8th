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
	_float3 worldShadowPosition{ 140.f,200.f,170.f };
	_float3 worldShadowLookAt{ 130.f,0.f,160.f };

	XMStoreFloat4x4(&m_ShadowCameraContext.viewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&worldShadowPosition), XMLoadFloat3(&worldShadowLookAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&m_ShadowCameraContext.projMatrix, XMMatrixPerspectiveFovLH(math::ToRadian(90.f), 1600.f / 900.f, 0.1f, 1000.f));

	return S_OK;
}

void CameraManager::Update()
{
	/*Update inverse matrix & cam position*/
	if (!m_pMainCamera)
		return;

	m_MainCameraContext.viewMatrix = m_pMainCamera->GetViewMatrix();
	m_MainCameraContext.projMatrix = m_pMainCamera->GetProjMatrix();

	XMStoreFloat4x4(&m_MainCameraContext.viewMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MainCameraContext.viewMatrix)));

	XMStoreFloat4x4(&m_MainCameraContext.projMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_MainCameraContext.projMatrix)));

	_float4x4 viewMatrixInverse = m_MainCameraContext.viewMatrixInverse;
	m_MainCameraContext.camPosition = _float3{ viewMatrixInverse._41,viewMatrixInverse._42,viewMatrixInverse._43 };

	m_MainCameraContext.farZ = m_pMainCamera->GetFarZ();
	m_MainCameraContext.nearZ = m_pMainCamera->GetNearZ();
}

void CameraManager::SetMainCamera(const _string& cameraTag)
{
	auto iter = m_Cameras.find(cameraTag);

	if (iter == m_Cameras.end())
		MSG_BOX("Invalid camera tag");

	Safe_Release(m_pMainCamera);

	m_pMainCamera = iter->second;
	m_pMainCamera->AddRef();
}

void CameraManager::SetShadowCamera(const _string& cameraTag)
{
	auto iter = m_Cameras.find(cameraTag);

	if (iter == m_Cameras.end())
		MSG_BOX("Invalid camera tag");

	Safe_Release(m_pMainCamera);

	m_pShadowCamera = iter->second;
	m_pShadowCamera->AddRef();
}

void CameraManager::AddCamera(const _string& cameraTag, CameraComponent* component)
{
	m_Cameras.emplace(cameraTag, component);
	component->AddRef();
}

_float3 CameraManager::WorldToScreen(_float3 worldPosition)
{
	auto engine = EngineCore::GetInstance();
	D3D11_VIEWPORT viewPort = engine->GetViewport();

	_vector screenPos = XMVector3Project(XMLoadFloat3(&worldPosition),
		static_cast<_float>(viewPort.TopLeftX),
		static_cast<_float>(viewPort.TopLeftY),
		static_cast<_float>(viewPort.Width),
		static_cast<_float>(viewPort.Height),
		static_cast<_float>(viewPort.MinDepth),
		static_cast<_float>(viewPort.MaxDepth),
		XMLoadFloat4x4(&m_MainCameraContext.projMatrix),
		XMLoadFloat4x4(&m_MainCameraContext.viewMatrix),
		XMMatrixIdentity());

	_float3 screen{};
	XMStoreFloat3(&screen, screenPos);

	return screen;
}

void CameraManager::Free()
{
	__super::Free();

	for (auto& pair : m_Cameras)
		Safe_Release(pair.second);

	Safe_Release(m_pMainCamera);
}

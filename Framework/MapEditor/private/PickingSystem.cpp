#include "MapEditorPCH.h"
#include "PickingSystem.h"
#include "Object.h"
#include "PickableComponent.h"
#include "EngineCore.h"

PickingSystem::PickingSystem()
{
}

PickingSystem* PickingSystem::Create()
{
	PickingSystem* Instance = new PickingSystem();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PickingSystem::Initialize()
{
	return S_OK;
}

void PickingSystem::Update()
{
	if (ImGuizmo::IsUsing())
		return;

	auto engine = EngineCore::GetInstance();
	CAMERA_CONTEXT camContext = engine->GetCameraContext();

	_float3 camPosition = camContext.camPosition;
	_float3 direction;

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(engine->GetWindowHandle(), &mousePos);
	_float3 mouse{ static_cast<_float>(mousePos.x),static_cast<_float>(mousePos.y),0.f };
	_float4x4 projMat = camContext.projMatrix;
	_float4x4 viewMat = camContext.viewMatrix;
	D3D11_VIEWPORT viewPort{};
	_uint numView = 1;
	engine->GetDeviceContext()->RSGetViewports(&numView, &viewPort);

	XMStoreFloat3(&mouse, XMVector3Unproject(XMVectorSetW(XMLoadFloat3(&mouse), 1.f),
		viewPort.TopLeftX, viewPort.TopLeftY, viewPort.Width, viewPort.Height,
		viewPort.MinDepth, viewPort.MaxDepth,
		XMLoadFloat4x4(&projMat),
		XMLoadFloat4x4(&viewMat),
		XMMatrixIdentity()));

	XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&mouse) - XMLoadFloat3(&camPosition)));

	RAY ray{ camPosition,direction };
	RayCast(ray);
}

void PickingSystem::RegisterComponent(PickableComponent* component)
{
	m_Pickables.push_back(component);
	component->AddRef();
}

void PickingSystem::UnRegisterComponent(PickableComponent* component)
{
	_uint currSize = m_Pickables.size();

	m_Pickables.remove(component);
	if(currSize != m_Pickables.size())
		Safe_Release(component);
}

void PickingSystem::Free()
{
	for (auto& object : m_Pickables)
		Safe_Release(object);
	m_Pickables.clear();
}

void PickingSystem::RayCast(RAY ray)
{
	PICK_RESULT result{};
	m_LastPickResult = result;

	for (const auto& pickable : m_Pickables)
	{
		result = pickable->IntersectRay(ray);
		
		if (result.isHit && result.distance < m_LastPickResult.distance)
		{
			m_LastPickResult = result;
			//m_LastPickResult.origin = ray.position;
			//m_LastPickResult.direction = ray.direction;
		}
	}
}

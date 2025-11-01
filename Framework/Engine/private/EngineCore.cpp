#include "EnginePCH.h"
#include "EngineCore.h"

//manager
#ifdef USE_IMGUI
#include "ImGuiManager.h"
#endif

#include "SoundManager.h"
#include "TimerManager.h"
#include "ObjectManager.h"
#include "PrototypeManager.h"
#include "ResourceManager.h"
#include "TaskManager.h"
#include "LevelManager.h"
#include "CameraManager.h"
#include "RenderTargetManager.h"

//system
#include "RenderSystem.h"
#include "InputSystem.h"
#include "LightSystem.h"
#include "NavigationSystem.h"
#include "CollisionSystem.h"

//utill
#include "GraphicDevice.h"
#include "Random.h"

IMPLEMENT_SINGLETON(EngineCore);

EngineCore::EngineCore()
{
}

HRESULT EngineCore::Initialize(const EngineDESC& desc)
{
	m_hWnd = desc.hWnd;

	m_pGraphicDevice = GraphicDevice::Create(desc.hWnd,desc.winMode,desc.winSizeX,desc.winSizeY,&m_Viewport);
	if (!m_pGraphicDevice)
		return E_FAIL;

	m_pRenderTargetManager = RenderTargetManager::Create();
	if (!m_pRenderTargetManager)
		return E_FAIL;

	m_pRenderSystem = RenderSystem::Create();
	if (!m_pRenderSystem)
		return E_FAIL;

	m_pSoundManager = SoundManager::Create();
	if (!m_pSoundManager)
		return E_FAIL;

	m_pTimerManager = TimerManager::Create();
	if (!m_pTimerManager)
		return E_FAIL;

	m_pRandom = Random::Create();
	if (!m_pRandom)
		return E_FAIL;

	m_pInputSystem = InputSystem::Create(desc.hWnd);
	if (!m_pInputSystem)
		return E_FAIL;

	m_pLevelManager = LevelManager::Create();
	if (!m_pLevelManager)
		return E_FAIL;

	m_pPrototypeManager = PrototypeManager::Create(desc.levelCnt);
	if (!m_pPrototypeManager)
		return E_FAIL;

	m_pObjectManager = ObjectManager::Create(desc.levelCnt);
	if (!m_pObjectManager)
		return E_FAIL;

	m_pTaskManager = TaskManager::Create();
	if (!m_pTaskManager)
		return E_FAIL;

	m_pCameraManager = CameraManager::Create();
	if (!m_pCameraManager)
		return E_FAIL;

	m_pResourceManager = ResourceManager::Create(desc.levelCnt);
	if (!m_pResourceManager)
		return E_FAIL;

	m_pLightManager = LightSystem::Create();
	if (!m_pLightManager)
		return E_FAIL;

	m_pNavigationSystem = NavigationSystem::Create();
	if (!m_pNavigationSystem)
		return E_FAIL;

	m_pCollisionSystem = CollisionSystem::Create(desc.numCollisionFilter);
	if (!m_pCollisionSystem)
		return E_FAIL;

#ifdef USE_IMGUI
	GUIState state{};
	state.pObjectManager = m_pObjectManager;
	state.pCameraManager = m_pCameraManager;
	state.pLevelManager = m_pLevelManager;

	m_pImGuiManager = ImGuiManager::Create(m_hWnd, m_pGraphicDevice->GetDevice(),m_pGraphicDevice->GetDeviceContext(),state);
	if (!m_pImGuiManager)
		return E_FAIL;
#endif

	return S_OK;
}

void EngineCore::Free()
{
	__super::Free();

	Safe_Release(m_pRandom);
	Safe_Release(m_pRenderTargetManager);
	Safe_Release(m_pRenderSystem);
	Safe_Release(m_pTimerManager);

#ifdef USE_IMGUI
	Safe_Release(m_pImGuiManager);
#endif

	Safe_Release(m_pLevelManager);
	Safe_Release(m_pTaskManager);
	Safe_Release(m_pPrototypeManager);
	Safe_Release(m_pResourceManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pCameraManager);
	Safe_Release(m_pInputSystem);
	Safe_Release(m_pLightManager);
	Safe_Release(m_pNavigationSystem);
	Safe_Release(m_pCollisionSystem);
	Safe_Release(m_pGraphicDevice);	/*GraphicDevice는 가장 먼저 생성, 가장 마지막 해제*/

}

void EngineCore::Tick(_float dt)
{
	m_pInputSystem->Update();

	m_pSoundManager->Update();

	m_pObjectManager->PriorityUpdate(dt);
	m_pObjectManager->Update(dt);
	m_pCollisionSystem->Update();
	m_pCameraManager->Update();
	m_pObjectManager->LateUpdate(dt);

	
	m_pLevelManager->Update(dt);
	m_pLevelManager->Render(); /*Debug Only*/

	std::vector<std::vector<RenderProxy>> proxies(ENUM_CLASS(RenderGroup::Count));
	std::vector<LightProxy> lights;

	if (FAILED(m_pObjectManager->ExtractRenderProxies(proxies)))
		return;
	if (FAILED(m_pLightManager->ExtractLightProxy(lights)))
		return;

	if (m_NavDebugEnable)
	{
		if(FAILED(m_pNavigationSystem->ExtractDebugProxies(proxies[ENUM_CLASS(RenderGroup::NavMeshDebug)])))
			return;
	}

	if (m_ColliderDebugEnable)
	{
		if (FAILED(m_pCollisionSystem->ExtractDebugProxies(proxies[ENUM_CLASS(RenderGroup::ColliderDebug)])))
			return;
	}

	m_pRenderSystem->Submit(std::move(proxies), lights);

	BeginRender();
	Render();
	EndRender();

	m_pInputSystem->EndFrame();
}


#ifdef USE_IMGUI
#pragma region ImGui
_bool EngineCore::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_pImGuiManager->WndProcHandler(hWnd, msg, wParam, lParam);
}
void EngineCore::AddPanel(IPanel* panel)
{
	m_pImGuiManager->AddPanel(panel);
}
#pragma endregion
#endif

#pragma region Timer
HRESULT EngineCore::AddTimer(const std::string& timerTag)
{
	return m_pTimerManager->AddTimer(timerTag);
}

void EngineCore::UpdateTimer(const std::string& timerTag)
{
	m_pTimerManager->Update(timerTag);
}

_float EngineCore::GetDeltaTime(const std::string& timerTag)
{
	return m_pTimerManager->GetDeltaTime(timerTag);
}

#pragma endregion

#pragma region Sound
void EngineCore::Load3DSound(const _string& key, const _string& filePath, _bool loop)
{
	m_pSoundManager->Load3DSound(key, filePath, loop);
}
void EngineCore::Load2DSound(const _string& key, const _string& filePath, _bool loop)
{
	m_pSoundManager->Load2DSound(key, filePath, loop);
}
FMOD::Channel* EngineCore::PlaySFX(const _string& soundTag)
{
	return m_pSoundManager->PlaySFX(soundTag);
}
void EngineCore::PlayBGM(const _string& soundTag)
{
	m_pSoundManager->PlayBGM(soundTag);
}
#pragma endregion

#pragma region GraphicDevice
ID3D11Device* EngineCore::GetDevice()
{
	return m_pGraphicDevice->GetDevice();
}

ID3D11DeviceContext* EngineCore::GetDeviceContext()
{
	return m_pGraphicDevice->GetDeviceContext();
}

#pragma endregion

#pragma region CameraManager
void EngineCore::AddCamera(const _string& cameraTag, CameraComponent* component)
{
	m_pCameraManager->AddCamera(cameraTag, component);
}
void EngineCore::SetMainCamera(const _string& cameraTag)
{
	m_pCameraManager->SetMainCamera(cameraTag);
}
CAMERA_CONTEXT EngineCore::GetCameraContext()
{
	return m_pCameraManager->GetCameraContext();
}
#pragma endregion

#pragma region Resource
HRESULT EngineCore::LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer)
{
	return m_pResourceManager->LoadBuffer(levelID,key,pBuffer);
}
HRESULT EngineCore::LoadShaderFromFile(const _string& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
	return m_pResourceManager->LoadShaderFromFile(filePath,key,pElement,numElement);
}
HRESULT EngineCore::LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key)
{
	return m_pResourceManager->LoadMaterialFromJson(levelID, filePath, key);
}
HRESULT EngineCore::LoadModelFromFile(_uint levelID, const _string& filePath, const _string& key)
{
	return m_pResourceManager->LoadModelFromFile(levelID, filePath, key);
}
HRESULT EngineCore::LoadAnimationSetFromFile(_uint levelID, const _string& filePath, const _string& key)
{
	return m_pResourceManager->LoadAnimationSetFromFile(levelID, filePath, key);
}
HRESULT EngineCore::LoadNavMeshFromFile(_uint levelID, const _string& filePath, const _string& key)
{
	return m_pResourceManager->LoadNavMeshFromFile(levelID, filePath, key);
}
VIBuffer* EngineCore::GetBuffer(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetBuffer(levelID, key);
}

Shader* EngineCore::GetShader(const _string& key)
{
	return m_pResourceManager->GetShader(key);
}
Material* EngineCore::GetMaterial(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetMaterial(levelID, key);
}
Model* EngineCore::GetModel(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetModel(levelID, key);
}
ANIMATION_SET EngineCore::GetAnimation(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetAnimation(levelID, key);
}
NavMesh* EngineCore::GetNavMesh(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetNavMesh(levelID, key);
}
#pragma endregion

#pragma region Prototype
HRESULT EngineCore::AddPrototype(_uint level, const _string& prototypeTag, Object* prototype)
{
	return m_pPrototypeManager->AddPrototype(level, prototypeTag, prototype);
}

Object* EngineCore::ClonePrototype(_uint level, const _string& prototypeTag, InitDESC* arg)
{
	return m_pPrototypeManager->ClonePrototype(level, prototypeTag, arg);
}

#pragma endregion

#pragma region Object
HRESULT EngineCore::AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, InitDESC* arg, Object** out)
{
	return m_pObjectManager->AddObject(prototypeLevel, prototypeTag, layerLevel, layerTag, arg, out);
}

std::unordered_map<_string, Layer*>& EngineCore::GetLayers(_uint levelID)
{
	return m_pObjectManager->GetLayers(levelID);
}

Object* EngineCore::GetFrontObject(_uint layerLevel, const _string& layerTag)
{
	return m_pObjectManager->GetFrontObject(layerLevel, layerTag);
}

const std::list<class Object*>& EngineCore::GetObjects(_uint layerLevel, const _string& layerTag)
{
	return m_pObjectManager->GetObjects(layerLevel, layerTag);
}

#pragma endregion

#pragma region Level
void EngineCore::ChangeLevel(_uint nextLevelID, Level* nextLevel)
{
	m_pLevelManager->ChangeLevel(nextLevelID, nextLevel);
}

Level* EngineCore::GetCurrLevel() const
{
	return m_pLevelManager->GetCurrLevel();
}

void EngineCore::ClearResource(_uint levelID)
{
	m_pPrototypeManager->Clear(levelID);
	m_pObjectManager->Clear(levelID);
	m_pResourceManager->Clear(levelID);
}

#pragma endregion

#pragma region RenderSystem
Renderer* EngineCore::GetRenderer() const
{
	return m_pRenderSystem->GetRenderer();
}
#pragma endregion

#pragma region InputSystem
void EngineCore::OnKeyEvent(const RAWKEYBOARD& keyboard)
{
	m_pInputSystem->OnKeyEvent(keyboard);
}
void EngineCore::OnMouseEvent(const RAWMOUSE& mouse)
{
	m_pInputSystem->OnMouseEvent(mouse);
}
void EngineCore::SetMouseDelta(_float dx, _float dy)
{
	m_pInputSystem->SetMouseDelta(dx, dy);
}
_float2 EngineCore::GetMouseDelta() const
{
	return m_pInputSystem->GetMouseDelta();
}
_bool EngineCore::IsKeyDown(WPARAM key) const
{
	return m_pInputSystem->IsKeyDown(key);
}
_bool EngineCore::IsKeyPressed(WPARAM key) const
{
	return m_pInputSystem->IsKeyPressed(key);
}
_bool EngineCore::IsKeyRelease(WPARAM key) const
{
	return m_pInputSystem->IsKeyRelease(key);
}
_bool EngineCore::IsKeyAway(WPARAM key) const
{
	return m_pInputSystem->IsKeyAway(key);
}
_bool EngineCore::IsMouseDown(MouseButton button) const
{
	return m_pInputSystem->IsMouseDown(button);
}
_bool EngineCore::IsMousePress(MouseButton button) const
{
	return m_pInputSystem->IsMousePressed(button);
}
_bool EngineCore::IsMouseRelease(MouseButton button) const
{
	return m_pInputSystem->IsMouseRelease(button);
}
_bool EngineCore::IsMouseAway(MouseButton button) const
{
	return m_pInputSystem->IsMouseAway(button);
}
#pragma endregion

#pragma region Navigation System
void EngineCore::RegisterNavigation(NavigationComponent* component)
{
	m_pNavigationSystem->RegisterNavigation(component);
}
HRESULT EngineCore::SetNavMesh(_uint levelID, const _string& navMeshTag)
{
	return m_pNavigationSystem->SetNavMesh(levelID, navMeshTag);
}
#pragma endregion

#pragma region LightSystem
void EngineCore::RegisterLight(LightComponent* light)
{
	m_pLightManager->RegisterLight(light);
}
void EngineCore::UnRegisterLight(LightComponent* light)
{
	m_pLightManager->UnRegisterLight(light);
}
#pragma endregion

#pragma region CollisionSystem
void EngineCore::RegisterCollider(ColliderComponent* component)
{	
	m_pCollisionSystem->RegisterCollider(component);
}
void EngineCore::UnRegisterCollider(ColliderComponent* component)
{
	m_pCollisionSystem->UnRegisterCollider(component);
}
void EngineCore::AddColliderFilterGroup(_uint left, _uint right)
{
	m_pCollisionSystem->AddColliderFilterGroup(left, right);
}
RAYCAST_DATA EngineCore::RayCast(RAY worldRay, _uint rayFilter)
{
	return m_pCollisionSystem->RayCast(worldRay, rayFilter);
}
RAYCAST_DATA EngineCore::RayCast(RAY worldRay, _float maxDistance, _uint rayFilter)
{
	return m_pCollisionSystem->RayCast(worldRay, maxDistance, rayFilter);
}
#pragma endregion

#pragma region RenderTargetManager
HRESULT EngineCore::AddRenderTarget(const _string& targetTag, _uint width, _uint height, DXGI_FORMAT format, _float4 clearColor)
{
	return m_pRenderTargetManager->AddRenderTarget(targetTag, width, height, format, clearColor);
}
HRESULT EngineCore::AddMRT(const _string& mrtTag, const _string& targetTag)
{
	return m_pRenderTargetManager->AddMRT(mrtTag, targetTag);
}
HRESULT EngineCore::BindShaderResource(Shader* shader, const _string& targetTag, const _string& constantName)
{
	return m_pRenderTargetManager->BindShaderResource(shader, targetTag, constantName);
}
HRESULT EngineCore::BeginMRT(const _string& mrtTag)
{
	return m_pRenderTargetManager->BeginMRT(mrtTag);
}
HRESULT EngineCore::EndMRT()
{
	return m_pRenderTargetManager->EndMRT();
}
#pragma endregion



LRESULT EngineCore::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto engine = EngineCore::GetInstance();
	
#ifdef USE_IMGUI
	if (engine->WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
#endif

	switch (msg)
	{
	case WM_INPUT:
	{
		_uint size = 0;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
		std::vector<BYTE> buf(size);

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buf.data(), &size, sizeof(RAWINPUTHEADER)) != size)
			break;

		RAWINPUT* input = (RAWINPUT*)buf.data();
		if (input->header.dwType == RIM_TYPEMOUSE)
		{
			const RAWMOUSE& m = input->data.mouse;

			_float dx = (_float)input->data.mouse.lLastX;
			_float dy = (_float)input->data.mouse.lLastY;

			engine->SetMouseDelta(dx, dy);
			engine->OnMouseEvent(m);
		}
		else if (input->header.dwType == RIM_TYPEKEYBOARD)
		{
			const RAWKEYBOARD& k = input->data.keyboard;

			if (k.VKey == VK_ESCAPE)
				DestroyWindow(hWnd);

			engine->OnKeyEvent(k);
		}
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

HRESULT EngineCore::BeginRender()
{
	if (FAILED(m_pGraphicDevice->ClearBackBufferView()))
		return E_FAIL;

	if (FAILED(m_pGraphicDevice->ClearDepthStencilView()))
		return E_FAIL;

	return S_OK;
}

HRESULT EngineCore::Render()
{
	if (FAILED(m_pRenderSystem->RenderLoop()))
		return E_FAIL;

#ifdef USE_IMGUI
	m_pImGuiManager->BeginFrame();
	m_pImGuiManager->Render();
	m_pImGuiManager->EndFrame();
#endif

	return S_OK;
}

HRESULT EngineCore::EndRender()
{
	return m_pGraphicDevice->Present();
}

#include "EnginePCH.h"
#include "EngineCore.h"

#ifdef USE_IMGUI
#include "ImGuiManager.h"
#endif

#include "SoundManager.h"
#include "RenderSystem.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
#include "ObjectManager.h"
#include "TaskManager.h"
#include "PrototypeManager.h"
#include "ResourceManager.h"
#include "LevelManager.h"
#include "Random.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(EngineCore);

EngineCore::EngineCore()
{
}

HRESULT EngineCore::Initialize(const EngineDESC& desc)
{
	hWnd = desc.hWnd;

	m_pGraphicDevice = GraphicDevice::Create(desc.hWnd,desc.winMode,desc.winSizeX,desc.winSizeY);
	if (!m_pGraphicDevice)
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

#ifdef USE_IMGUI
	m_pImGuiManager = ImGuiManager::Create(hWnd, m_pGraphicDevice->GetDevice(),m_pGraphicDevice->GetDeviceContext());
	if (!m_pImGuiManager)
		return E_FAIL;
#endif

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

	m_pPipeLine = PipeLine::Create();
	if (!m_pPipeLine)
		return E_FAIL;

	m_pResourceManager = ResourceManager::Create(desc.levelCnt);
	if (!m_pResourceManager)
		return E_FAIL;

	return S_OK;
}

void EngineCore::Free()
{
	__super::Free();

	Safe_Release(m_pRandom);
	Safe_Release(m_pRenderSystem);
	Safe_Release(m_pTimerManager);

#ifdef USE_IMGUI
	Safe_Release(m_pImGuiManager);
#endif

	Safe_Release(m_pLevelManager);
	Safe_Release(m_pTaskManager);
	Safe_Release(m_pPrototypeManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pResourceManager);
	Safe_Release(m_pGraphicDevice);	/*GraphicDevice는 가장 먼저 생성, 가장 마지막 해제*/

}

void EngineCore::Tick(_float dt)
{
	m_pSoundManager->Update();

	m_pObjectManager->Update(dt);
	m_pObjectManager->LateUpdate(dt);
	
	m_pLevelManager->Update(dt);
	m_pLevelManager->Render();

	std::vector<std::vector<RenderProxy>> proxies(ENUM_CLASS(RenderGroup::Count));
	if (FAILED(m_pObjectManager->ExtractRenderProxies(proxies)))
		return;
	m_pRenderSystem->Submit(std::move(proxies));

	BeginRender();

#ifdef USE_IMGUI
	m_pImGuiManager->BeginFrame();
	m_pImGuiManager->Render();
	m_pImGuiManager->EndFrame();
#endif

	Render();
	EndRender();
}


#ifdef USE_IMGUI
#pragma region ImGui
_bool EngineCore::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_pImGuiManager->WndProcHandler(hWnd, msg, wParam, lParam);
}
void EngineCore::AddImGuiWindow(const _string& tag, std::function<void()> window)
{
	ImGuiManager::ImGuiWindowDESC desc{};
	desc.windowTag = tag;
	desc.window = window;

	m_pImGuiManager->AddImGuiWindow(desc);
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
void EngineCore::LoadSound(const std::string& key, const std::string& filepath, bool loop)
{
	m_pSoundManager->LoadSound(key, filepath, loop);
}

void EngineCore::PlaySFX(const std::string& key)
{
	m_pSoundManager->PlaySFX(key);
}

void EngineCore::PlayBGM(const std::string& key)
{
	m_pSoundManager->PlayBGM(key);
}

void EngineCore::StopSound(const std::string& key)
{
	m_pSoundManager->Stop(key);
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

#pragma region Resource
HRESULT EngineCore::LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer)
{
	return m_pResourceManager->LoadBuffer(levelID,key,pBuffer);
}
HRESULT EngineCore::LoadShader(_uint levelID, const _string& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
	return m_pResourceManager->LoadShader(levelID,filePath,key,pElement,numElement);
}
VIBuffer* EngineCore::GetBuffer(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetBuffer(levelID, key);
}

Shader* EngineCore::GetShader(_uint levelID, const _string& key)
{
	return m_pResourceManager->GetShader(levelID, key);
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
HRESULT EngineCore::AddObject(_uint prototypeLevel, const _string& prototypeTag, _uint layerLevel, const _string& layerTag, InitDESC* arg)
{
	return m_pObjectManager->AddObject(prototypeLevel, prototypeTag, layerLevel, layerTag, arg);
}

#pragma endregion

#pragma region Level
void EngineCore::ChangeLevel(_uint levelID, Level* nextLevel)
{
	m_pLevelManager->ChangeLevel(levelID, nextLevel);
}

void EngineCore::ClearResource(_uint levelID)
{
	m_pPrototypeManager->Clear(levelID);
	m_pObjectManager->Clear(levelID);
	m_pResourceManager->Clear(levelID);
}

#pragma endregion

#pragma region Rendering
Renderer* EngineCore::GetRenderer() const
{
	return m_pRenderSystem->GetRenderer();
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
	return m_pRenderSystem->Render();;
}

HRESULT EngineCore::EndRender()
{
	return m_pGraphicDevice->Present();
}
#pragma endregion


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

	m_pGraphicDevice = GraphicDevice::Create(desc.hWnd,desc.winMode,desc.winSizeX,desc.winSizeY);
	if (!m_pGraphicDevice)
		return E_FAIL;

#ifdef USE_IMGUI
	m_pImGuiManager = ImGuiManager::Create();
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

	return S_OK;
}

void EngineCore::Free()
{
	__super::Free();

	Safe_Release(m_pRandom);
	Safe_Release(m_pRenderSystem);
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pGraphicDevice);

#ifdef USE_IMGUI
	Safe_Release(m_pImGuiManager);
#endif

	Safe_Release(m_pLevelManager);
	Safe_Release(m_pTaskManager);
	Safe_Release(m_pPrototypeManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pSoundManager);
	Safe_Release(m_pPipeLine);

}

void EngineCore::Tick(_float dt)
{
	m_pSoundManager->Update();

	m_pObjectManager->Update(dt);
	m_pObjectManager->LateUpdate(dt);
	
	m_pLevelManager->Update(dt);
	m_pLevelManager->Render();

	BeginDraw();
	Draw();
	EndDraw();
}

#ifdef USE_IMGUI
#pragma region ImGui

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

#pragma region Prototype
HRESULT EngineCore::AddPrototype(_uint level, const _string& prototypeTag, Base* prototype)
{
	return m_pPrototypeManager->AddPrototype(level, prototypeTag, prototype);
}

Base* EngineCore::ClonePrototype(Prototype type, _uint level, const _string& prototypeTag, InitDESC* arg)
{
	return m_pPrototypeManager->ClonePrototype(type, level, prototypeTag, arg);
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
}

#pragma endregion

#pragma region Rendering
HRESULT EngineCore::BeginDraw()
{
	if (FAILED(m_pGraphicDevice->ClearBackBufferView()))
		return E_FAIL;

	if (FAILED(m_pGraphicDevice->ClearDepthStencilView()))
		return E_FAIL;

	return S_OK;
}

HRESULT EngineCore::Draw()
{
	//render loop

	return S_OK;
}

HRESULT EngineCore::EndDraw()
{
	return m_pGraphicDevice->Present();
}

#pragma endregion


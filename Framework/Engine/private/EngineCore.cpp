#include "EnginePCH.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "RenderSystem.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
#include "LevelManager.h"
#include "Random.h"

IMPLEMENT_SINGLETON(EngineCore);

EngineCore::EngineCore()
{
}

HRESULT EngineCore::Initialize(const EngineDESC& desc)
{
	hWnd = desc.hWnd;

	renderSystem = RenderSystem::Create();
	if (!renderSystem)
		return E_FAIL;

	soundManager = SoundManager::Create();
	if (!soundManager)
		return E_FAIL;

	timerManager = TimerManager::Create();
	if (!timerManager)
		return E_FAIL;

	random = Random::Create();
	if (!random)
		return E_FAIL;

	graphicDevice = GraphicDevice::Create(desc.hWnd,desc.winMode,desc.winSizeX,desc.winSizeY);
	if (!graphicDevice)
		return E_FAIL;

	levelManager = LevelManager::Create();
	if (!levelManager)
		return E_FAIL;

	return S_OK;
}

void EngineCore::Free()
{
	__super::Free();

	Safe_Release(random);
	Safe_Release(renderSystem);
	Safe_Release(timerManager);
	Safe_Release(soundManager);
	Safe_Release(graphicDevice);
	Safe_Release(levelManager);
}

void EngineCore::Tick(_float dt)
{
	soundManager->Update();
	levelManager->Update(dt);

	BeginDraw();
	Draw();
	EndDraw();
}

HRESULT EngineCore::AddTimer(const std::string& timerTag)
{
	return timerManager->AddTimer(timerTag);
}

void EngineCore::UpdateTimer(const std::string& timerTag)
{
	timerManager->Update(timerTag);
}

_float EngineCore::GetDeltaTime(const std::string& timerTag)
{
	return timerManager->GetDeltaTime(timerTag);
}

ID3D11Device* EngineCore::GetDevice()
{
	return graphicDevice->GetDevice();
}

ID3D11DeviceContext* EngineCore::GetDeviceContext()
{
	return graphicDevice->GetDeviceContext();
}

void EngineCore::ChangeLevel(Level* nextLevel)
{
	levelManager->ChangeLevel(nextLevel);
}

HRESULT EngineCore::BeginDraw()
{
	if (FAILED(graphicDevice->ClearBackBufferView()))
		return E_FAIL;

	if (FAILED(graphicDevice->ClearDepthStencilView()))
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
	return graphicDevice->Present();
}

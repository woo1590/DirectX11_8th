#include "EnginePCH.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "RenderSystem.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
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
}

void EngineCore::Tick(_float dt)
{
	soundManager->Update();

	DrawBegin();
	Draw();
	DrawEnd();
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

HRESULT EngineCore::DrawBegin()
{
	if (FAILED(graphicDevice->ClearBackBufferView(&clearColor)))
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

HRESULT EngineCore::DrawEnd()
{
	return graphicDevice->Present();
}

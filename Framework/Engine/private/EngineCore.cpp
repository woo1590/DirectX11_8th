#include "EnginePCH.h"
#include "EngineCore.h"
#include "SoundManager.h"
#include "RenderSystem.h"
#include "TimerManager.h"
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

	return S_OK;
}

void EngineCore::Free()
{
	__super::Free();

	Safe_Release(random);
	Safe_Release(renderSystem);
	Safe_Release(timerManager);
	Safe_Release(soundManager);
}

HRESULT EngineCore::AddTimer(const std::string& timerTag)
{
	return timerManager->AddTimer(timerTag);
}

void EngineCore::Update(const std::string& timerTag)
{
	timerManager->Update(timerTag);
}

_float EngineCore::GetDeltaTime(const std::string& timerTag)
{
	return timerManager->GetDeltaTime(timerTag);
}

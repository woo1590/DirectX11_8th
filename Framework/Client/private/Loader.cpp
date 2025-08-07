#include "pch.h"
#include "Loader.h"

Loader::Loader()
{
}

Loader* Loader::Create(LevelID levelID)
{
	Loader* Instance = new Loader();

	if (FAILED(Instance->Initialize(levelID)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Loader::Initialize(LevelID levelID)
{
	worker = std::thread(&Loader::Loading, this);
	return S_OK;
}

void Loader::Free()
{
	__super::Free();

	if (worker.joinable())
		worker.join();
}

HRESULT Loader::Loading()
{
	HRESULT hr{};

	mtx.lock();
	switch (levelID)
	{
	case Client::LevelID::Logo:
		hr = LoadingForLogoLevel();
		break;
	case Client::LevelID::GamePlay:
		hr = LoadingForGamePlayLevel();
		break;
	default:
		break;
	}
	mtx.unlock();

	if (FAILED(hr))
		return E_FAIL;

	isFinished.store(true);
	return S_OK;
}

HRESULT Loader::LoadingForLogoLevel()
{
	return S_OK;
}

HRESULT Loader::LoadingForGamePlayLevel()
{
	return S_OK;
}

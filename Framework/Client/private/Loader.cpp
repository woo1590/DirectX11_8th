#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"
#include "TestCube.h"
#include "BackGround.h"

Loader::Loader()
{
}

Loader* Loader::Create(LevelID nextLevelID)
{
	Loader* Instance = new Loader();

	if (FAILED(Instance->Initialize(nextLevelID)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Loader::Initialize(LevelID nextLevelID)
{
	worker = std::thread(&Loader::Loading, this);
	this->nextLevelID = nextLevelID;
	return S_OK;
}

void Loader::Free()
{
	__super::Free();

	if (worker.joinable())
		worker.join();
}

void Loader::DebugPrint()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), debugText.c_str());
}

HRESULT Loader::Loading()
{
	HRESULT hr{};

	mtx.lock();
	switch (nextLevelID)
	{
	case Client::LevelID::Logo:
		hr = LoadingForLogo();
		break;
	case Client::LevelID::GamePlay:
		hr = LoadingForGamePlay();
		break;
	case Client::LevelID::Test:
		hr = LoadingForTest();
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

HRESULT Loader::LoadingForLogo()
{
	return S_OK;
}

HRESULT Loader::LoadingForGamePlay()
{
	return S_OK;
}

HRESULT Loader::LoadingForTest()
{
	auto engine = EngineCore::GetInstance();

	/*Load Sound*/
	engine->LoadSound("TestBGM", "../bin/resource/TestBGM.mp3", true);

	/*Prototype Object*/

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Test), "Prototype_Object_TestCube", TestCube::Create())))
		return E_FAIL;

	if(FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Test),"Prototype_Object_BackGround",BackGround::Create())))
		return E_FAIL;

	return S_OK;
}

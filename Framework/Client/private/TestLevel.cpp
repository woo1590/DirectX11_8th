#include "pch.h"
#include "TestLevel.h"
#include "EngineCore.h"

TestLevel::TestLevel()
{
}

TestLevel* TestLevel::Create()
{
	TestLevel* Instance = new TestLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestLevel::Initialize()
{
	if (FAILED(InitLayerBackGround("Layer_BackGround")))
		return E_FAIL;

	if (FAILED(InitLayerTest("Layer_Test")))
		return E_FAIL;

	EngineCore::GetInstance()->PlayBGM("TestBGM2");

	return S_OK;
}

void TestLevel::Free()
{
	__super::Free();

	EngineCore::GetInstance()->StopSound("TestBGM2");

}

void TestLevel::Update(_float dt)
{
	if (GetAsyncKeyState(VK_SPACE))
		EngineCore::GetInstance()->StopSound("TestBGM2");
}

HRESULT TestLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"Test Level");

	return S_OK;
}

HRESULT TestLevel::InitLayerBackGround(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Test), "Prototype_Object_BackGround",
		ENUM_CLASS(LevelID::Test), layerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT TestLevel::InitLayerTest(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Test), "Prototype_Object_TestCube",
		ENUM_CLASS(LevelID::Test), layerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

#include "pch.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "LoadingLevel.h"

LogoLevel::LogoLevel()
{
}

LogoLevel* LogoLevel::Create()
{
	LogoLevel* Instance = new LogoLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT LogoLevel::Initialize()
{
	if (FAILED(InitializeLayerBackground("Layer_Background")))
		return E_FAIL;

	return S_OK;
}

void LogoLevel::Free()
{
	__super::Free();
}

void LogoLevel::Update(_float dt)
{
	if (GetAsyncKeyState(VK_SPACE))
		EngineCore::GetInstance()->ChangeLevel(ENUM_CLASS(LevelID::Loading), LoadingLevel::Create(LevelID::GamePlay));
}

HRESULT LogoLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"Logo Level");

	return S_OK;
}

HRESULT LogoLevel::InitializeLayerBackground(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround",
		ENUM_CLASS(LevelID::Logo), layerTag, nullptr)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LoadinAnim",
		ENUM_CLASS(LevelID::Logo), layerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

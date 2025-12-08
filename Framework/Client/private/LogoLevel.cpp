#include "pch.h"
#include "EngineCore.h"
#include "LogoLevel.h"
#include "LoadingLevel.h"
#include "Command_ChangeLevel.h"

//object
#include "BackGround.h"
#include "PointLight.h"

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
	if (FAILED(Initialize_LayerHome("Layer_Home")))
		return E_FAIL;

	if (FAILED(Initialize_LayerCamera("Layer_Camera")))
		return E_FAIL;

	if (FAILED(Initialize_LayerCharacter("Layer_Character")))
		return E_FAIL;

	if (FAILED(Initialize_LayerLight("Layer_Light")))
		return E_FAIL;

	if (FAILED(Initialize_LayerUI("Layer_UI")))
		return E_FAIL;

	m_iBGMChannelID = EngineCore::GetInstance()->Play2DSound("BGM_Logo", 0.6f);

	return S_OK;
}

void LogoLevel::Free()
{
	__super::Free();

	EngineCore::GetInstance()->StopSound(m_iBGMChannelID);
}

void LogoLevel::Update(_float dt)
{
	if (GetAsyncKeyState(VK_RETURN))
	{
		EngineCore::GetInstance()->RegisterCommand(Command_ChangeLevel::Create(LevelID::Loading, LoadingLevel::Create(LevelID::Stage1)));
	}
}

HRESULT LogoLevel::Render()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), L"Logo Level");

	return S_OK;
}

HRESULT LogoLevel::Initialize_LayerHome(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SelectHome", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT LogoLevel::Initialize_LayerCharacter(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();
	
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoPanda", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoPlayer", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT LogoLevel::Initialize_LayerCamera(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_FreeCam", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoShadowCam", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT LogoLevel::Initialize_LayerLight(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	PointLight::POINT_LIGHT_DESC desc{};
	desc.color = _float4{ 1.f,0.3f,0.f,1.f };
	desc.range = 30.f;
	desc.position = _float3{ -72.f,61.8f,-179.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Logo), layerTag,&desc)))
		return E_FAIL;

	desc.range = 20.f;
	desc.position = _float3{ -174.25f,18.f,-155.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Logo), layerTag,&desc)))
		return E_FAIL;
	
	desc.range = 420.f;
	desc.position = _float3{ -130.25f,338.f,0.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Logo), layerTag,&desc)))
		return E_FAIL;

	desc.color = _float4{ 0.f,0.5,1.f,1.f };
	desc.range = 480.f;
	desc.position = _float3{ -130.f,338.f,0.f };
	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", ENUM_CLASS(LevelID::Logo), layerTag,&desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT LogoLevel::Initialize_LayerUI(const _string& layerTag)
{
	auto engine = EngineCore::GetInstance();

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Button", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;

	if (FAILED(engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_MouseCursor", ENUM_CLASS(LevelID::Logo), layerTag)))
		return E_FAIL;

	return S_OK;
}

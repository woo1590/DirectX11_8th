#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"
#include "TestCube.h"
#include "VIBufferQuad.h"
#include "Shader.h"
#include "BackGround.h"
#include "TransformComponent.h"

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
	m_Worker = std::thread(&Loader::Loading, this);
	this->m_eNextLevelID = nextLevelID;
	return S_OK;
}

void Loader::Free()
{
	__super::Free();

	if (m_Worker.joinable())
		m_Worker.join();
}

void Loader::DebugPrint()
{
	SetWindowText(EngineCore::GetInstance()->GetWindowHandle(), m_strDebugText.c_str());
}

HRESULT Loader::Loading()
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	HRESULT hr{};

	switch (m_eNextLevelID)
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
	
	if (FAILED(hr))
		return E_FAIL;

	m_isFinished.store(true);

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
	m_strDebugText = L"사운드 로딩중..";
	engine->LoadSound("TestBGM2", "../bin/resource/TestBGM2.mp3", true);

	/*Load Resource*/
	if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad", VIBufferQuad::Create())))
		return E_FAIL;

	if (FAILED(engine->LoadShader(ENUM_CLASS(LevelID::Static), "../bin/shaderfiles/Shader_VtxTex.hlsl", "Shader_VtxTex", VTXTEX::elements, VTXTEX::numElement)))
		return E_FAIL;

	/*Prototype Object*/
	m_strDebugText = L"객체원형 로딩중..";
	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Test), "Prototype_Object_TestCube", TestCube::Create())))
		return E_FAIL;

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Test), "Prototype_Object_BackGround", BackGround::Create())))
		return E_FAIL;

	m_strDebugText = L"로딩완료";

	return S_OK;
}

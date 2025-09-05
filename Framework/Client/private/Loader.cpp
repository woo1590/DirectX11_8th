#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"

//object
#include "BackGround.h"
#include "TestCube.h"
#include "FreeCam.h"
#include "LoadingAnim.h"
#include "Terrain.h"

//component
#include "TransformComponent.h"

//resource
#include "VIBufferQuad.h"
#include "VIBufferCube.h"
#include "VIBufferTerrain.h"
#include "Shader.h"

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
	CoInitializeEx(nullptr, 0);

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
	auto engine = EngineCore::GetInstance();

	/*Load Sound*/
	m_strDebugText = L"사운드 로딩중..";
	engine->LoadSound("TestBGM2", "../bin/resource/TestBGM2.mp3", true);

	/*Load Shader*/
	m_strDebugText = L"셰이더 로딩중..";
	if (FAILED(engine->LoadShaderFromFile(ENUM_CLASS(LevelID::Static), "../bin/shaderfiles/Shader_VtxTex.hlsl", "Shader_VtxTex",
		VTXTEX::elements, VTXTEX::numElement)))
		return E_FAIL;

	if (FAILED(engine->LoadShaderFromFile(ENUM_CLASS(LevelID::Static), "../bin/shaderfiles/Shader_VtxCube.hlsl", "Shader_VtxCube",
		VTXCUBE::elements, VTXCUBE::numElement)))
		return E_FAIL;

	if (FAILED(engine->LoadShaderFromFile(ENUM_CLASS(LevelID::Static), "../bin/shaderfiles/Shader_VtxNorTex.hlsl", "Shader_VtxNorTex",
		VTXNORTEX::elements, VTXNORTEX::numElement)))
		return E_FAIL;

	/*Load Resource*/
	m_strDebugText = L"리소스 로딩중..";
	if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad", VIBufferQuad::Create())))
		return E_FAIL;

	if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Cube", VIBufferCube::Create())))
		return E_FAIL;

	if(FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static),"Buffer_Terrain",VIBufferTerrain::Create("../bin/resource/terrain/Height1.bmp"))))
		return E_FAIL;

	if (FAILED(engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/background/background.png", 1, "Texture_Background")))
		return E_FAIL;

	if (FAILED(engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/loading/frame_107_stand_%d.png", 100, "Texture_Anim")))
		return E_FAIL;

	if (FAILED(engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/TestCube.dds", 1, "Texture_TestCube")))
		return E_FAIL;

	if (FAILED(engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/terrain/Tile0.jpg", 1, "Texture_Terrain")))
		return E_FAIL;

	/*Load Prototype Object*/
	m_strDebugText = L"객체원형 로딩중..";

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_TestCube", TestCube::Create())))
		return E_FAIL;

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BackGround", BackGround::Create())))
		return E_FAIL;

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_LoadinAnim", LoadingAnim::Create())))
		return E_FAIL;

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_FreeCam", FreeCam::Create())))
		return E_FAIL;

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Terrain", Terrain::Create())))
		return E_FAIL;

	m_strDebugText = L"로딩완료";


	return S_OK;
}

HRESULT Loader::LoadingForGamePlay()
{
	m_strDebugText = L"로딩완료";
	return S_OK;
}

HRESULT Loader::LoadingForTest()
{
	
	return S_OK;
}

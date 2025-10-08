#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"

//object
#include "BackGround.h"
#include "FreeCam.h"
#include "Terrain.h"
#include "Sun.h"
#include "Player.h"
#include "PlayerCam.h"
#include "Hand.h"
#include "Cameleon.h"
#include "ConcealedAmmo.h"
#include "IcySpear.h"

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

	/*Load Buffer*/
	m_strDebugText = L"리소스 로딩중..";
	
	/*Load Material*/
	if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Logo), "../bin/resource/materials/loading.json", "Mtrl_Background_Logo")))
		return E_FAIL;

	/*Load Prototype Object*/
	m_strDebugText = L"객체원형 로딩중..";

	if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Logo), "Prototype_Object_BackGround", BackGround::Create())))
		return E_FAIL;

	m_strDebugText = L"로딩완료";


	return S_OK;
}

HRESULT Loader::LoadingForGamePlay()
{
	auto engine = EngineCore::GetInstance();

	/*Load Sound*/
	m_strDebugText = L"사운드 로딩중..";

	/*Load Shader*/
	m_strDebugText = L"셰이더 로딩중..";

	/*Load Models*/
	{
		m_strDebugText = L"리소스 로딩중..";
		if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::GamePlay), "Buffer_Terrain", VIBufferTerrain::Create("../bin/resource/textures/terrain/Height.bmp"))))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/playerhand/playerhand.model",
			"Model_PlayerHand")))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/cameleon/cameleon.model",
			"Model_Weapon_Cameleon")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/concealed_ammo/concealed_ammo.model",
			"Model_Weapon_ConcealedAmmo")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/icy_spear/icy_spear.model",
			"Model_Weapon_IcySpear")))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/stage1/stage1_wall0.model",
			"Model_Stage1_Wall0")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/boss/boss_stage.model",
			"Model_BossStage")))
			return E_FAIL;
	}


	/*Load Animation Set*/
	{
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/test.animationset",
			"AnimationSet_Test")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/playerhand.animationset", "AnimationSet_PlayerHand")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/cameleon.animationset",
			"AnimationSet_Weapon_Cameleon")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/concealed_ammo.animationset",
			"AnimationSet_Weapon_ConcealedAmmo")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/icy_spear.animationset",
			"AnimationSet_Weapon_IcySpear")))
			return E_FAIL;
	}
	/*Load Material*/
	if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/materials/terrain.json", "Mtrl_Terrain")))
		return E_FAIL;

	/*Load Prototype Object*/
	{
		m_strDebugText = L"객체원형 로딩중..";

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_FreeCam", FreeCam::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Terrain", Terrain::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Sun", Sun::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Player", Player::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_PlayerCam", PlayerCam::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Hand", Hand::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Cameleon", Cameleon::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ConcealedAmmo", ConcealedAmmo::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_IcySpear", IcySpear::Create())))
			return E_FAIL;
	}
	m_strDebugText = L"로딩완료";

	return S_OK;
}

HRESULT Loader::LoadingForTest()
{
	
	return S_OK;
}

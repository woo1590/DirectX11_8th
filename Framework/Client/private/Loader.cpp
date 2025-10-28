#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"

//object
#include "BackGround.h"
#include "ThirdCam.h"
#include "Terrain.h"
#include "Sun.h"

//player
#include "Player.h"
#include "PlayerCam.h"
#include "Hand.h"

//ui
#include "Sight.h"

//weapon
#include "Cameleon.h"
#include "ConcealedAmmo.h"
#include "IcySpear.h"
#include "PoisonousGhost.h"
#include "Foundry.h"
#include "DefaultBullet.h"	

//enemy
#include "Boss.h"
#include "Bomber.h"
#include "HorseHead.h"
#include "HorseHead_Shield.h"
#include "HorseHead_Sword.h"
#include "HorseHead_Head.h"
#include "Soldier.h"

//map
#include "SkyBox.h"
#include "BossStage.h"
#include "StaticMapObject.h"
#include "Door_L.h"
#include "Door_R.h"
#include "Door.h"
#include "EnemySpawner.h"

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
	if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Logo), "../bin/resource/materials/background_logo.json", "Mtrl_Background_Logo")))
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

	/*Load Navigation*/
	{
		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/data/navigation/stage1_navigation.dat", "NavMesh_Test")))
			return E_FAIL;
	}

	/*Load Texture*/
	{
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/materials/sight.json", "Mtrl_Sight")))
			return E_FAIL;
	}

	/*Load Models*/
	{
		m_strDebugText = L"리소스 로딩중..";
		if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::GamePlay), "Buffer_Terrain", VIBufferTerrain::Create("../bin/resource/textures/terrain/Height.bmp"))))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/playerhand/playerhand.model",
			"Model_PlayerHand")))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/enemy/horse_head/horse_head.model",
			"Model_Enemy_HorseHead")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/enemy/boss/boss.model",
			"Model_Enemy_Boss")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/enemy/bomber/bomber.model",
			"Model_Enemy_Bomber")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/enemy/soldier/soldier.model",
			"Model_Enemy_Soldier")))
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

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/poisonous_ghost/poisonous_ghost.model",
			"Model_Weapon_PoisonousGhost")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/foundry/foundry.model",
			"Model_Weapon_Foundry")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/weapon/projectile/default_bullet/default_bullet.model",
			"Model_Projectile_Default_Bullet")))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/stage1/stage1_wall0.model",
			"Model_Stage1_Wall0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/stage1/stage1_wall1.model",
			"Model_Stage1_Wall1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/stage1/stage1_wall2.model",
			"Model_Stage1_Wall2")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/floor_tile/floor_tile0.model",
			"Model_Floor_Tile0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/floor_tile/floor_tile1.model",
			"Model_Floor_Tile1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/floor_tile/floor_tile2.model",
			"Model_Floor_Tile2")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/floor_tile/floor_tile3.model",
			"Model_Floor_Tile3")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/celling/celling_side.model",
			"Model_Celling_Side")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/celling/celling_corner.model",
			"Model_Celling_Corner")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/celling/celling0.model",
			"Model_Celling0")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/reliefwall/reliefwall0.model",
			"Model_ReliefWall0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/reliefwall/reliefwall1.model",
			"Model_ReliefWall1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/reliefwall/reliefwall2.model",
			"Model_ReliefWall2")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/reliefwall/reliefwall3.model",
			"Model_ReliefWall3")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/door/door0.model",
			"Model_Door0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/door/door1.model",
			"Model_Door1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/door/door2.model",
			"Model_Door2")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/door/door_l.model",
			"Model_Door_L")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/door/door_r.model",
			"Model_Door_R")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/stair/stair0.model",
			"Model_Stair0")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/obstacle/obstacle0.model",
			"Model_Obstacle0")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/models/map/boss/boss_stage.model",
			"Model_Boss_Stage")))
			return E_FAIL;
	}

	/*Load Animation Set*/
	{
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/test.animationset",
			"AnimationSet_Test")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/playerhand.animationset",
			"AnimationSet_PlayerHand")))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/horse_head.animationset",
			"AnimationSet_Enemy_HorseHead")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/boss.animationset",
			"AnimationSet_Enemy_Boss")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/bomber.animationset",
			"AnimationSet_Enemy_Bomber")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/soldier.animationset",
			"AnimationSet_Enemy_Soldier")))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/cameleon.animationset",
			"AnimationSet_Weapon_Cameleon")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/concealed_ammo.animationset",
			"AnimationSet_Weapon_ConcealedAmmo")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/icy_spear.animationset",
			"AnimationSet_Weapon_IcySpear")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/poisonous_ghost.animationset",
			"AnimationSet_Weapon_PoisonousGhost")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/animationsets/foundry.animationset",
			"AnimationSet_Weapon_Foundry")))
			return E_FAIL;
	}
	/*Load Material*/
	{
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/materials/terrain.json", "Mtrl_Terrain")))
			return E_FAIL;

		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::GamePlay), "../bin/resource/materials/skybox.json", "Mtrl_Skybox")))
			return E_FAIL;
	}

	/*Load Prototype Object*/
	{
		m_strDebugText = L"객체원형 로딩중..";

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ThirdCam", ThirdCam::Create())))
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

		/*UI*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Sight", Sight::Create())))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead", HorseHead::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Shield", HorseHead_Shield::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Sword", HorseHead_Sword::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_HorseHead_Head", HorseHead_Head::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss", Boss::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Bomber", Bomber::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Soldier", Soldier::Create())))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Cameleon", Cameleon::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ConcealedAmmo", ConcealedAmmo::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_IcySpear", IcySpear::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_PoisonousGhost", PoisonousGhost::Create())))
			return E_FAIL;

		if(FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay),"Prototype_Object_Foundry",Foundry::Create())))
			return E_FAIL;

		if(FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay),"Prototype_Object_Default_Bullet",DefaultBullet::Create())))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Skybox", SkyBox::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Boss_Stage", BossStage::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Stage1_Wall0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Stage1_Wall1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Stage1_Wall2", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Floor_Tile0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Floor_Tile1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Floor_Tile2", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Floor_Tile3", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Celling_Corner", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Celling_Side", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Celling0", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ReliefWall0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ReliefWall1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ReliefWall2", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_ReliefWall3", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door2", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door_L", Door_L::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door_R", Door_R::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Door", Door::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Stair0", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_Obstacle0", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_EnemySpawner", EnemySpawner::Create())))
			return E_FAIL;
	}
	m_strDebugText = L"로딩완료";

	return S_OK;
}

HRESULT Loader::LoadingForTest()
{
	
	return S_OK;
}

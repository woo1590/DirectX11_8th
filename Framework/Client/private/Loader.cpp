#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"

//object
#include "BackGround.h"
#include "ShadowCam.h"
#include "Terrain.h"
#include "Sun.h"

//player
#include "Player.h"
#include "PlayerCam.h"
#include "Hand.h"

//ui
#include "Crosshair.h"
#include "PlayerPanel.h"
#include "PlayerIcon.h"
#include "WeaponPanel.h"
#include "WeaponIcon.h"
#include "WeaponSlot.h"
#include "SlotNum.h"
#include "SkillPanel.h"
#include "DashIcon.h"
#include "Bar.h"
#include "EffectBackground.h"
#include "CountNumber.h"
#include "Cross.h"
#include "DamageFont.h"
#include "BossIcon.h"
#include "BossHpPanel.h"
#include "EnemyHpPanel.h"

//weapon
#include "Cameleon.h"
#include "ConcealedAmmo.h"
#include "IcySpear.h"
#include "PoisonousGhost.h"
#include "Foundry.h"
#include "Prism.h"

#include "DefaultBullet.h"
#include "PrismProjectile.h"
#include "Dynamite.h"

//item
#include "DropWeapon.h"
#include "Ammo.h"
#include "Dumpling.h"
#include "Coin.h"
#include "Chest.h"

//enemy
#include "Fracture.h"

#include "Boss.h"
#include "Boss_Core.h"
#include "Boss_RightArm.h"
#include "Boss_LeftArm.h"
#include "Boss_Head.h"
#include "BossPillar.h"
#include "BossArmProjectile.h"
#include "BossStoneProjectile.h"

#include "Bomber.h"

#include "HorseHead.h"
#include "HorseHead_Shield.h"
#include "HorseHead_Sword.h"
#include "HorseHead_Head.h"

#include "Soldier.h"
#include "Soldier_Head.h"
#include "Soldier_Sword.h"

#include "SpearMan.h"
#include "SpearMan_Head.h"
#include "SpearMan_Spear.h"

#include "CrossbowMan.h"
#include "CrossbowMan_Head.h"

//effect
#include "EffectContainer.h"
#include "SpriteEffect.h"

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
	case Client::LevelID::Test:
		hr = LoadingForTest();
		break;
	case Client::LevelID::Stage1:
		hr = LoadingForStage1();
		break;
	case Client::LevelID::StageBoss:
		hr = LoadingForBossStage();
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

	/*Load Navigation*/
	{
		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::Static), "../bin/data/navigation/stage1_navigation.dat", "NavMesh_Stage1")))
			return E_FAIL;

		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::Static), "../bin/data/navigation/boss_navigation.dat", "NavMesh_StageBoss")))
			return E_FAIL;
	}

	/*Load Models*/
	{
		m_strDebugText = L"리소스 로딩중..";
		if (FAILED(engine->LoadBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Terrain", VIBufferTerrain::Create("../bin/resource/textures/terrain/Height.bmp"))))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/playerhand/playerhand2.model",
			"Model_PlayerHand")))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/horse_head/horse_head.model",
			"Model_Enemy_HorseHead")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/bomber/bomber.model",
			"Model_Enemy_Bomber")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/soldier/soldier.model",
			"Model_Enemy_Soldier")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/spear_man/spear_man.model",
			"Model_Enemy_SpearMan")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/crossbow_man/crossbow_man.model",
			"Model_Enemy_CrossbowMan")))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/cameleon/cameleon.model",
			"Model_Weapon_Cameleon")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/concealed_ammo/concealed_ammo.model",
			"Model_Weapon_ConcealedAmmo")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/icy_spear/icy_spear.model",
			"Model_Weapon_IcySpear")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/poisonous_ghost/poisonous_ghost.model",
			"Model_Weapon_PoisonousGhost")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/foundry/foundry.model",
			"Model_Weapon_Foundry")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/prism/prism.model",
			"Model_Weapon_Prism")))
			return E_FAIL;

		/*Drop Weapon*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/foundry/drop_foundry.model",
			"Model_DropWeapon_Foundry")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/concealed_ammo/drop_concealed_ammo.model",
			"Model_DropWeapon_ConcealedAmmo")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/poisonous_ghost/drop_poisonous_ghost.model",
			"Model_DropWeapon_PoisonousGhost")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/prism/drop_prism.model",
			"Model_DropWeapon_Prism")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/cameleon/drop_cameleon.model",
			"Model_DropWeapon_Cameleon")))
			return E_FAIL;

		/*Item*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/item/coin/coin.model",
			"Model_Item_Coin")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/item/ammo/ammo.model",
			"Model_Item_Ammo")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/item/dumpling/dumpling.model",
			"Model_Item_Dumpling")))
			return E_FAIL;


		/*Projectile*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/projectile/default_bullet/default_bullet.model",
			"Model_Projectile_Default_Bullet")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/projectile/dynamite/dynamite.model",
			"Model_Projectile_Dynamite")))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/door/door_l.model",
			"Model_Door_L")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/door/door_r.model",
			"Model_Door_R")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage1_area0/stage1_area0.model",
			"Model_Stage1_Area0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage1_area1/stage1_area1.model",
			"Model_Stage1_Area1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage1_area2/stage1_area2.model",
			"Model_Stage1_Area2")))
			return E_FAIL;

		/*Map*/

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/boss2/boss_stage.model",
			"Model_Boss_Stage")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/chest/chest.model",
			"Model_Chest")))
			return E_FAIL;
	}

	/*Load Fracture Model*/
	{
		for (_uint i = 0; i < 17; ++i)
		{
			_string filePath = "soldier" + std::to_string(i) + ".model";
			_string modelTag = "Soldier" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/soldier/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}

		for (_uint i = 0; i < 16; ++i)
		{
			_string filePath = "horse_head" + std::to_string(i) + ".model";
			_string modelTag = "HorseHead" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/horse_head/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}

		for (_uint i = 0; i < 17; ++i)
		{
			_string filePath = "spear_man" + std::to_string(i) + ".model";
			_string modelTag = "SpearMan" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/spear_man/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}

		for (_uint i = 0; i < 12; ++i)
		{
			_string filePath = "crossbow_man" + std::to_string(i) + ".model";
			_string modelTag = "CrossbowMan" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/crossbow_man/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}
	}


	/*Load Animation Set*/
	{
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/test.animationset",
			"AnimationSet_Test")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/playerhand2.animationset",
			"AnimationSet_PlayerHand")))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/horse_head.animationset",
			"AnimationSet_Enemy_HorseHead")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/bomber.animationset",
			"AnimationSet_Enemy_Bomber")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/soldier.animationset",
			"AnimationSet_Enemy_Soldier")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/spear_man.animationset",
			"AnimationSet_Enemy_SpearMan")))
			return E_FAIL;
		
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/crossbow_man.animationset",
			"AnimationSet_Enemy_CrossbowMan")))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/cameleon.animationset",
			"AnimationSet_Weapon_Cameleon")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/concealed_ammo.animationset",
			"AnimationSet_Weapon_ConcealedAmmo")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/icy_spear.animationset",
			"AnimationSet_Weapon_IcySpear")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/poisonous_ghost.animationset",
			"AnimationSet_Weapon_PoisonousGhost")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/foundry.animationset",
			"AnimationSet_Weapon_Foundry")))
			return E_FAIL;

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/prism.animationset",
			"AnimationSet_Weapon_Prism")))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/chest.animationset",
			"AnimationSet_Chest")))
			return E_FAIL;
	}

	/*Load Material*/
	{

		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/sight.json", "Mtrl_Sight")))
			return E_FAIL;

		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/terrain.json", "Mtrl_Terrain")))
			return E_FAIL;

		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/skybox.json", "Mtrl_Skybox")))
			return E_FAIL;

		/*player panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/player_panel.json", "Mtrl_PlayerPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/player_icon.json", "Mtrl_PlayerIcon")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/bar_background.json", "Mtrl_Bar_Background")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/bar_background2.json", "Mtrl_Bar_Background2")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/bar_shield.json", "Mtrl_Bar_Shield")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/bar_hurt.json", "Mtrl_Bar_Hurt")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/bar_hp.json", "Mtrl_Bar_HP")))
			return E_FAIL;

		/*weapon panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_panel.json", "Mtrl_WeaponPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_slot_num1.json", "Mtrl_WeaponSlot_Num1")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_slot_num2.json", "Mtrl_WeaponSlot_Num2")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_slot_num3.json", "Mtrl_WeaponSlot_Num3")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_icon_foundry.json", "Mtrl_WeaponIcon_Foundry")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_icon_concealed_ammo.json", "Mtrl_WeaponIcon_ConcealedAmmo")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_icon_cameleon.json", "Mtrl_WeaponIcon_Cameleon")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_icon_prism.json", "Mtrl_WeaponIcon_Prism")))
			return E_FAIL;

		/*skill panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/skill_panel.json", "Mtrl_SkillPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/dash0.json", "Mtrl_Dash0")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/dash1.json", "Mtrl_Dash1")))
			return E_FAIL;

		/*effect background*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/background_effect.json", "Mtrl_EffectBackground")))
			return E_FAIL;

		/*number*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/num_hit.json", "Mtrl_NumHit")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/num_count.json", "Mtrl_NumCount")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/num_cross.json", "Mtrl_NumCross")))
			return E_FAIL;

		/*boss hp panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_hp_panel.json", "Mtrl_BossHpPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_hp0.json", "Mtrl_BossHp0")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_hp1.json", "Mtrl_BossHp1")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_hp2.json", "Mtrl_BossHp2")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_icon.json", "Mtrl_BossIcon")))
			return E_FAIL;

		/*enemy hp panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/enemy_hp_panel.json", "Mtrl_EnemyHpPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/enemy_hp_background.json", "Mtrl_EnemyBackground")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/enemy_hp_background2.json", "Mtrl_EnemyBackground2")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/enemy_hp0.json", "Mtrl_EnemyHp0")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/enemy_hp1.json", "Mtrl_EnemyHp1")))
			return E_FAIL;
	}

	/*Load Effect Prefab*/
	{
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MuzzleRed",
			EffectContainer::Create("../bin/resource/textures/effect/muzzle_red/muzzle_red.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MuzzleOrange",
			EffectContainer::Create("../bin/resource/textures/effect/muzzle_orange/muzzle_orange.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismFire",
			EffectContainer::Create("../bin/resource/textures/effect/prism_fire/prism_fire.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpawnSmoke",
			EffectContainer::Create("../bin/resource/textures/effect/spawn_smoke/spawn_smoke.json"))))
			return E_FAIL;
	}

	/*Load Prototype Object*/
	{
		m_strDebugText = L"객체원형 로딩중..";

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_ShadowCam", ShadowCam::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Terrain", Terrain::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Sun", Sun::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Player", Player::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerCam", PlayerCam::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Hand", Hand::Create())))
			return E_FAIL;

		/*UI*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Crosshair", Crosshair::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerPanel", PlayerPanel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerIcon", PlayerIcon::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", Bar::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponPanel", WeaponPanel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponIcon", WeaponIcon::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponSlot", WeaponSlot::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SlotNum", SlotNum::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SkillPanel", SkillPanel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DashIcon", DashIcon::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EffectBackground", EffectBackground::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", CountNumber::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cross", Cross::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", DamageFont::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossHpPanel", BossHpPanel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossIcon", BossIcon::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyHpPanel", EnemyHpPanel::Create())))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HorseHead", HorseHead::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HorseHead_Shield", HorseHead_Shield::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HorseHead_Sword", HorseHead_Sword::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HorseHead_Head", HorseHead_Head::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bomber", Bomber::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Soldier", Soldier::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Soldier_Head", Soldier_Head::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Soldier_Sword", Soldier_Sword::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpearMan", SpearMan::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpearMan_Head", SpearMan_Head::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpearMan_Spear", SpearMan_Spear::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CrossbowMan", CrossbowMan::Create())))
			return E_FAIL;

		/*Weapon*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cameleon", Cameleon::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_ConcealedAmmo", ConcealedAmmo::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_IcySpear", IcySpear::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PoisonousGhost", PoisonousGhost::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Foundry", Foundry::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Prism", Prism::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Default_Bullet", DefaultBullet::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismProjectile", PrismProjectile::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Dynamite", Dynamite::Create())))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Skybox", SkyBox::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Door_L", Door_L::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Door_R", Door_R::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Door", Door::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area2", StaticMapObject::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemySpawner", EnemySpawner::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", Chest::Create())))
			return E_FAIL;

		/*Fracture*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Fracture", Fracture::Create())))
			return E_FAIL;

		/*Item*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DropWeapon", DropWeapon::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Ammo", Ammo::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Coin", Coin::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Dumpling", Dumpling::Create())))
			return E_FAIL;

		
	}
	m_strDebugText = L"로딩완료";

	return S_OK;
}

HRESULT Loader::LoadingForTest()
{
	return S_OK;
}

HRESULT Loader::LoadingForStage1()
{
	return S_OK;
}

HRESULT Loader::LoadingForBossStage()
{
	auto engine = EngineCore::GetInstance();

	/*Load Model*/
	{
		/*Map*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/map/boss2/boss_stage.model",
			"Model_Boss_Stage")))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss/boss.model",
			"Model_Enemy_Boss")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_pillar/boss_pillar.model",
			"Model_BossPillar")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_stone_projectile/boss_stone_projectile0.model",
			"Model_Boss_Stone_Projectile0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_stone_projectile/boss_stone_projectile1.model",
			"Model_Boss_Stone_Projectile1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_arm_projectile/boss_arm_projectile_l.model",
			"Model_Boss_Arm_Projectile_L")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_arm_projectile/boss_arm_projectile_r.model",
			"Model_Boss_Arm_Projectile_R")))
			return E_FAIL;
	}
	/*Load AnimationSet*/
	{
		/*Enemy*/
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/animationsets/golem.animationset",
			"AnimationSet_Enemy_Boss")))
			return E_FAIL;
	}
	/*Load Prototype*/
	{
		/*Map*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Boss_Stage", BossStage::Create())))
			return E_FAIL;

		/*Enemy*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss", Boss::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss_Core", Boss_Core::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss_RightArm", Boss_RightArm::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss_LeftArm", Boss_LeftArm::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_Boss_Head", Boss_Head::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_BossPillar", BossPillar::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_BossStoneProjectile", BossStoneProjectile::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_BossArmProjectile", BossArmProjectile::Create())))
			return E_FAIL;
	}

	return S_OK;
}

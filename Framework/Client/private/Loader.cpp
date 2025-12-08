#include "pch.h"
#include "EngineCore.h"
#include "Loader.h"

//object
#include "BackGround.h"
#include "ShadowCam.h"
#include "Terrain.h"
#include "Sun.h"
#include "Decal.h"
#include "DefaultBulletTrail.h"
#include "PrismTrail.h"
#include "ItemGlow.h"
#include "AttackRange.h"
#include "BossStoneSmoke.h"
#include "BossStoneTrail.h"
#include "BossLaserTrail.h"
#include "BossLaserProjectile.h"
#include "BossLaserProjectileTrail.h"
#include "BossPillarSmoke.h"
#include "Boom.h"
#include "Cloud.h"
#include "Torch.h"
#include "PointLight.h"
#include "SelectHome.h"
#include "FreeCam.h"
#include "StageFont.h"
#include "LogoShadowCam.h"

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
#include "CoolDownEffect.h"
#include "HitCrossHair.h"
#include "InteractionUI.h"
#include "VictoryText.h"
#include "HitSight.h"
#include "OpenDoorMark.h"
#include "MiniMapPanel.h"
#include "MinimapLevel.h"
#include "MinimapTime.h"
#include "MinimapArea.h"
#include "LoadingPanel.h"
#include "Button.h"
#include "MouseCursor.h"

//weapon
#include "Cameleon.h"
#include "ConcealedAmmo.h"
#include "IcySpear.h"
#include "PoisonousGhost.h"
#include "Foundry.h"
#include "Prism.h"
#include "Hell.h"

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
#include "BossEye.h"

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

#include "Beetle.h"
#include "Beetle_Head.h"

//effect
#include "EffectContainer.h"
#include "SpriteEffect.h"
#include "ShieldHit.h"

//map
#include "SkyBox.h"
#include "BossStage.h"
#include "StaticMapObject.h"
#include "Door_L.h"
#include "Door_R.h"
#include "Door.h"
#include "EnemySpawner.h"
#include "SelectHome.h"
#include "LogoPlayer.h"
#include "LogoPanda.h"
#include "Barrel.h"
#include "BarrelBoom.h"

//component
#include "TransformComponent.h"

//resource
#include "VIBufferQuad.h"
#include "VIBufferCube.h"
#include "VIBufferTerrain.h"
#include "Shader.h"

Loader::Loader()
{
	// 메롱
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
	{
		engine->Load2DSound("BGM_Logo", "../bin/resource/sounds/bgm/logo_bgm.mp3", true);
		engine->Load2DSound("BGM_Normal", "../bin/resource/sounds/bgm/stage1_bgm.mp3", true);
		engine->Load2DSound("BGM_Boss", "../bin/resource/sounds/bgm/boss_bgm.mp3", true);
		engine->Load2DSound("BGM_Battle", "../bin/resource/sounds/bgm/bgm_battle.wav", true);
		engine->Load2DSound("BGM_BossStageEnter", "../bin/resource/sounds/bgm/boss_stage_enter.wav", true);
		engine->Load2DSound("SFX_DropWater", "../bin/resource/sounds/sfx/boss_stage_drop_water.wav", false);
		engine->Load2DSound("SFX_Stage", "../bin/resource/sounds/sfx/stage_sfx.wav", false);

		engine->Load2DSound("SFX_HitWeakness", "../bin/resource/sounds/sfx/hit_weakness.wav", false);
		engine->Load2DSound("SFX_Hit", "../bin/resource/sounds/sfx/enemy_hit.wav", false);
		engine->Load3DSound("SFX_HitEnemyShield", "../bin/resource/sounds/sfx/hit_enemy_shield.wav", false);
		engine->Load2DSound("SFX_EnemyDead", "../bin/resource/sounds/sfx/enemy_dead.wav", false);
		engine->Load2DSound("SFX_EnemySpread", "../bin/resource/sounds/sfx/enemy_spread.wav", false);
		engine->Load2DSound("SFX_PickUpCoin", "../bin/resource/sounds/sfx/pickup_coin.wav", false);
		engine->Load2DSound("SFX_ChestOpen", "../bin/resource/sounds/sfx/chest_open.wav", false);
		engine->Load2DSound("SFX_EnemySpawn", "../bin/resource/sounds/sfx/enemy_spawn.wav", false);
		engine->Load2DSound("SFX_PickUpAmmo", "../bin/resource/sounds/sfx/pickup_ammo.wav", false);
		engine->Load3DSound("SFX_DoorOpen", "../bin/resource/sounds/sfx/door_open.wav", false);
		engine->Load3DSound("SFX_MouseClick", "../bin/resource/sounds/sfx/mouse_click.wav", false);
		engine->Load3DSound("SFX_MouseHover", "../bin/resource/sounds/sfx/mouse_hover.wav", false);

		/*enemy*/
		engine->Load3DSound("SFX_HorseHeadAttack", "../bin/resource/sounds/sfx/horse_head_attack.wav", false);
		engine->Load3DSound("SFX_SoldierAttack0", "../bin/resource/sounds/sfx/soldier_attack0.wav", false);
		engine->Load3DSound("SFX_SpearManAttackReady", "../bin/resource/sounds/sfx/spearman_attack_ready.wav", false);
		engine->Load3DSound("SFX_SpearManAttack", "../bin/resource/sounds/sfx/spearman_attack.wav", false);
		engine->Load3DSound("SFX_CrossbowManReady", "../bin/resource/sounds/sfx/crossbowman_ready.wav", false);
		engine->Load3DSound("SFX_CrossbowManShot", "../bin/resource/sounds/sfx/crossbowman_shot.wav", false);
		engine->Load3DSound("SFX_BeetleRun", "../bin/resource/sounds/sfx/beetle_run.wav", false);

		/*player*/
		engine->Load2DSound("SFX_WeaponChange", "../bin/resource/sounds/sfx/weapon_change.wav", false);
		engine->Load2DSound("SFX_PlayerJump", "../bin/resource/sounds/sfx/player_jump.wav", false);
		engine->Load2DSound("SFX_PlayerJumpVoice", "../bin/resource/sounds/sfx/player_jump_voice.wav", false);
		engine->Load2DSound("SFX_PlayerLanding", "../bin/resource/sounds/sfx/player_landing.wav", false);
		engine->Load2DSound("SFX_PlayerWalk0", "../bin/resource/sounds/sfx/player_walk0.wav", false);
		engine->Load2DSound("SFX_PlayerWalk1", "../bin/resource/sounds/sfx/player_walk1.wav", false);
		engine->Load2DSound("SFX_PlayerDash", "../bin/resource/sounds/sfx/player_dash.wav", false);
		engine->Load2DSound("SFX_PlayerHitVoice0", "../bin/resource/sounds/sfx/player_hit_voice0.wav", false);
		engine->Load2DSound("SFX_PlayerHitVoice1", "../bin/resource/sounds/sfx/player_hit_voice1.wav", false);
		engine->Load2DSound("SFX_PlayerHit", "../bin/resource/sounds/sfx/player_hit.wav", false);

		/*weapon*/
		engine->Load2DSound("SFX_PrismFire", "../bin/resource/sounds/sfx/prism_fire.wav", false);
		engine->Load3DSound("SFX_PrismReflect", "../bin/resource/sounds/sfx/prism_reflect.wav", false);
		engine->Load2DSound("SFX_FoundryFire", "../bin/resource/sounds/sfx/foundry_fire.wav", false);
		engine->Load2DSound("SFX_FoundryReload0", "../bin/resource/sounds/sfx/foundry_reload0.wav", false);
		engine->Load2DSound("SFX_FoundryReload1", "../bin/resource/sounds/sfx/foundry_reload1.wav", false);
		engine->Load2DSound("SFX_FoundryReload2", "../bin/resource/sounds/sfx/foundry_reload2.wav", false);
		engine->Load2DSound("SFX_ConcealedAmmoFire", "../bin/resource/sounds/sfx/concealed_ammo_fire.wav", false);
		engine->Load2DSound("SFX_ConcealedAmmoSkill", "../bin/resource/sounds/sfx/concealed_ammo_skill.wav", false);
		engine->Load2DSound("SFX_ConcealedAmmoReload0", "../bin/resource/sounds/sfx/concealed_ammo_reload0.wav", false);
		engine->Load2DSound("SFX_ConcealedAmmoReload1", "../bin/resource/sounds/sfx/concealed_ammo_reload1.wav", false);
		engine->Load2DSound("SFX_CameleonReload", "../bin/resource/sounds/sfx/cameleon_reload.wav", false);
		engine->Load2DSound("SFX_CameleonFire", "../bin/resource/sounds/sfx/cameleon_fire.wav", false);
		engine->Load2DSound("SFX_HellPump", "../bin/resource/sounds/sfx/hell_pump.wav", false);
		engine->Load2DSound("SFX_HellFire", "../bin/resource/sounds/sfx/hell_fire.wav", false);
		engine->Load2DSound("SFX_HellReload", "../bin/resource/sounds/sfx/hell_reload.wav", false);
		engine->Load3DSound("SFX_DynamiteExplode", "../bin/resource/sounds/sfx/dynamite_explode.wav", false);

		/*boss*/
		engine->Load2DSound("SFX_BossLaser", "../bin/resource/sounds/sfx/boss_laser.wav", false);
		engine->Load2DSound("SFX_BossLaserShot", "../bin/resource/sounds/sfx/boss_laser_shot.wav", false);
		engine->Load2DSound("SFX_BossHitGround", "../bin/resource/sounds/sfx/boss_hit_ground.wav", false);
		engine->Load3DSound("SFX_BossPillarExplode", "../bin/resource/sounds/sfx/boss_pillar_explode.wav", false);
		engine->Load2DSound("SFX_BossPillarUp", "../bin/resource/sounds/sfx/boss_pillar_up.wav", false);
		engine->Load2DSound("SFX_BossStoneProjectileCharge", "../bin/resource/sounds/sfx/boss_stone_projectile_charge.wav", false);
		engine->Load3DSound("SFX_BossStoneExplode", "../bin/resource/sounds/sfx/boss_stone_explode.wav", false);
		engine->Load2DSound("SFX_BossArmShot", "../bin/resource/sounds/sfx/boss_arm_shot.wav", false);
		engine->Load2DSound("SFX_Victory", "../bin/resource/sounds/sfx/victory_sfx.wav", false);
		engine->Load2DSound("SFX_BossDead", "../bin/resource/sounds/sfx/boss_dead.wav", false);

	}

	/*Load Navigation*/
	{
		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::Static), "../bin/data/navigation/stage1_navigation.dat", "NavMesh_Stage1")))
			return E_FAIL;
		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::Static), "../bin/data/navigation/stage2_navigation.dat", "NavMesh_Stage2")))
			return E_FAIL;
		if (FAILED(engine->LoadNavMeshFromFile(ENUM_CLASS(LevelID::Static), "../bin/data/navigation/stage3_navigation.dat", "NavMesh_Stage3")))
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

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/beetle/beetle.model",
			"Model_Enemy_Beetle")))
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
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/weapon/hell/hell.model",
			"Model_Weapon_Hell")))
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
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/drop_weapon/hell/drop_hell.model",
			"Model_DropWeapon_Hell")))
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
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/boss_laser_projectile/boss_laser_projectile.model",
			"Model_BossLaserProjectile")))
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

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage2_area0/stage2_area0.model",
			"Model_Stage2_Area0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage2_area1/stage2_area1.model",
			"Model_Stage2_Area1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage2_area2/stage2_area2.model",
			"Model_Stage2_Area2")))
			return E_FAIL;

		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage3_area0/stage3_area0.model",
			"Model_Stage3_Area0")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage3_area1/stage3_area1.model",
			"Model_Stage3_Area1")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/stage3_area2/stage3_area2.model",
			"Model_Stage3_Area2")))
			return E_FAIL;

		/*Map*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/boss2/boss_stage.model",
			"Model_Boss_Stage")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/chest/chest.model",
			"Model_Chest")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/cloud/cloud.model",
			"Model_Cloud")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/area/logo/logo.model",
			"Model_SelectHome")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/logo_panda/logo_panda.model",
			"Model_LogoPanda")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/logo_player/logo_player.model",
			"Model_LogoPlayer")))
			return E_FAIL;  
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/map/barrel/barrel.model",
			"Model_Barrel")))
			return E_FAIL;  

		/*Effect*/
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/spearman_hit/spearman_hit.model",
			"Model_SpearmanHit")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_hit_ground/boss_hit_ground.model",
			"Model_BossHitGround")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_punch/boss_punch.model",
			"Model_BossPunch")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_stone_smoke/boss_stone_smoke.model",
			"Model_BossStoneSmoke")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_stone_trail/boss_stone_trail.model",
			"Model_BossStoneTrail")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_pillar_smoke/boss_pillar_smoke.model",
			"Model_BossPillarSmoke")))
			return E_FAIL;
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/effect/boss_weakness/boss_weakness.model",
			"Model_BossWeakness")))
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

		for (_uint i = 0; i < 2; ++i)
		{
			_string filePath = "beetle" + std::to_string(i) + ".model";
			_string modelTag = "Beetle" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/beetle/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}

		for (_uint i = 0; i < 11; ++i)
		{
			_string filePath = "bomber" + std::to_string(i) + ".model";
			_string modelTag = "Bomber" + std::to_string(i);
			if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/models/enemy/fracture/bomber/" + filePath,
				"Model_Fracture_" + modelTag)))
				return E_FAIL;
		}
	}

	/*Load Animation Set*/
	{
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/logo.animationset",
			"AnimationSet_SelectHome")))
			return E_FAIL;
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/logo_player.animationset",
			"AnimationSet_LogoPlayer")))
			return E_FAIL;
		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/logo_panda.animationset",
			"AnimationSet_LogoPanda")))
			return E_FAIL;

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

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/beetle.animationset",
			"AnimationSet_Enemy_Beetle")))
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

		if (FAILED(engine->LoadAnimationSetFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/animationsets/hell.animationset",
			"AnimationSet_Weapon_Hell")))
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
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/hit_cross_hair.json", "Mtrl_HitCrossHair")))
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
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/weapon_icon_hell.json", "Mtrl_WeaponIcon_Hell")))
			return E_FAIL;

		/*skill panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/skill_panel.json", "Mtrl_SkillPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/dash0.json", "Mtrl_Dash0")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/dash1.json", "Mtrl_Dash1")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/cooldown_effect.json", "Mtrl_CoolDownEffect")))
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

		/*interaction panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/interaction_frame.json", "Mtrl_InteractionFrame")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/interaction_key.json", "Mtrl_InteractionKey")))
			return E_FAIL;

		/*victory ui*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/victory_text.json", "Mtrl_VictoryText")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/victory_text_white.json", "Mtrl_VictoryTextWhite")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/victory_ring.json", "Mtrl_VictoryRing")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/victory_rock.json", "Mtrl_VictoryRock")))
			return E_FAIL;

		/*hit sight*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/hit_sight.json", "Mtrl_HitSight")))
			return E_FAIL;

		/*door mark*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/door_ring.json", "Mtrl_DoorRing")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/door_mark.json", "Mtrl_DoorMark")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/door_arrow.json", "Mtrl_DoorArrow")))
			return E_FAIL;

		/*minimap*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_panel.json", "Mtrl_MinimapPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_time.json", "Mtrl_MinimapTime")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_level.json", "Mtrl_MinimapLevel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_area.json", "Mtrl_MinimapArea")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_player_icon.json", "Mtrl_MinimapPlayerIcon")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/minimap_enemy_icon.json", "Mtrl_MinimapEnemyIcon")))
			return E_FAIL;

		/*loading panel*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/loading_panel.json", "Mtrl_LoadingPanel")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/loading_bar.json", "Mtrl_LoadingBar")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/loading_boss_icon.json", "Mtrl_LoadingBossIcon")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/loading_start_icon.json", "Mtrl_LoadingStartIcon")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/loading_stage_icon.json", "Mtrl_LoadingStageIcon")))
			return E_FAIL;

		/*button*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/button.json", "Mtrl_Button")))
			return E_FAIL;

		/*mouse cursor*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/mouse.json", "Mtrl_MouseCursor")))
			return E_FAIL;

		/*effect*/
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/effect_decal.json", "Mtrl_Decal")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/trail_bullet_red.json", "Mtrl_TrailBulletRed")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/trail_bullet_green.json", "Mtrl_TrailBulletGreen")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/trail_prism.json", "Mtrl_TrailPrism")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/item_glow.json", "Mtrl_ItemGlow")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/item_particle.json", "Mtrl_ItemParticle")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/effect_attack_range.json", "Mtrl_AttackRange")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_laser_trail.json", "Mtrl_BossLaserTrail")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/boss_laser_projectile_trail.json", "Mtrl_BossLaserProjectileTrail")))
			return E_FAIL;
		if (FAILED(engine->LoadMaterialFromJson(ENUM_CLASS(LevelID::Static), "../bin/resource/materials/shield_hit.json", "Mtrl_ShieldHit")))
			return E_FAIL;
		if (FAILED(engine->LoadTextureFromFile(ENUM_CLASS(LevelID::Static), "../bin/resource/textures/dissolve_mask.png")))
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
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitWall",
			EffectContainer::Create("../bin/resource/textures/effect/hit_wall/hit_wall.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismHitWall",
			EffectContainer::Create("../bin/resource/textures/effect/prism_hit_wall/prism_hit_wall.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyDeadParticle",
			EffectContainer::Create("../bin/resource/textures/effect/enemy_dead_particle/enemy_dead_particle.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Explode",
			EffectContainer::Create("../bin/resource/textures/effect/explode/explode.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DynamiteFire",
			EffectContainer::Create("../bin/resource/textures/effect/dynamite_fire/dynamite_fire.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyDeadSmoke",
			EffectContainer::Create("../bin/resource/textures/effect/enemy_dead_smoke/enemy_dead_smoke.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyHit",
			EffectContainer::Create("../bin/resource/textures/effect/enemy_hit/enemy_hit.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CameleonFireRed",
			EffectContainer::Create("../bin/resource/textures/effect/cameleon_fire_red/cameleon_fire_red.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CameleonFireGreen",
			EffectContainer::Create("../bin/resource/textures/effect/cameleon_fire_green/cameleon_fire_green.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CameleonHeadFire",
			EffectContainer::Create("../bin/resource/textures/effect/cameleon_head_fire/cameleon_head_fire.json"))))
			return E_FAIL;
		//if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_ShieldHit",
		//	EffectContainer::Create("../bin/resource/textures/effect/shield_hit/shield_hit.json"))))
		//	return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PrismHitEnemy",
			EffectContainer::Create("../bin/resource/textures/effect/prism_hit_enemy/prism_hit_enemy.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpearmanHit",
			EffectContainer::Create("../bin/resource/textures/effect/spearman_hit/spearman_hit.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossPillarSpawn",
			EffectContainer::Create("../bin/resource/textures/effect/boss_pillar_spawn/boss_pillar_spawn.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossPillarExplode",
			EffectContainer::Create("../bin/resource/textures/effect/boss_pillar_explode/boss_pillar_explode.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossHitGround",
			EffectContainer::Create("../bin/resource/textures/effect/boss_hit_ground/boss_hit_ground.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossPunch",
			EffectContainer::Create("../bin/resource/textures/effect/boss_punch/boss_punch.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossStoneExplode",
			EffectContainer::Create("../bin/resource/textures/effect/boss_stone_explode/boss_stone_explode.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossLaserCharge",
			EffectContainer::Create("../bin/resource/textures/effect/boss_laser_charge/boss_laser_charge.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossEyeSmoke",
			EffectContainer::Create("../bin/resource/textures/effect/boss_eye_smoke/boss_eye_smoke.json"))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_TorchFire",
			EffectContainer::Create("../bin/resource/textures/effect/torch_fire/torch_fire.json"))))
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

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cloud", Cloud::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PointLight", PointLight::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Torch", Torch::Create())))
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
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CoolDownEffect", CoolDownEffect::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitCrossHair", HitCrossHair::Create())))
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

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_InteractionUI", InteractionUI::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_VictoryText", VictoryText::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_HitSight", HitSight::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_OpenDoorMark", OpenDoorMark::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapPanel", MiniMapPanel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapLevel", MinimapLevel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapTime", MinimapTime::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MinimapArea", MinimapArea::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_LoadingPanel", LoadingPanel::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Button", Button::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_StageFont", StageFont::Create())))
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
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CrossbowMan_Head", CrossbowMan_Head::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Beetle", Beetle::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Beetle_Head", Beetle_Head::Create())))
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
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Hell", Hell::Create())))
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

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area0", StaticMapObject::Create(true))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area1", StaticMapObject::Create(true))))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage1_Area2", StaticMapObject::Create(true))))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage2_Area0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage2_Area1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage2_Area2", StaticMapObject::Create())))
			return E_FAIL;
		
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage3_Area0", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage3_Area1", StaticMapObject::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Stage3_Area2", StaticMapObject::Create(true))))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemySpawner", EnemySpawner::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Chest", Chest::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Barrel", Barrel::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BarrelBoom", BarrelBoom::Create())))
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

		/*effect*/
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Decal", Decal::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DefaultBulletTrail", DefaultBulletTrail::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_TrailPrism", PrismTrail::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_ItemGlow", ItemGlow::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_AttackRange", AttackRange::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossStoneSmoke", BossStoneSmoke::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossStoneTrail", BossStoneTrail::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossLaserTrail", BossLaserTrail::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossLaserProjectile", BossLaserProjectile::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossLaserProjectileTrail", BossLaserProjectileTrail::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_BossPillarSmoke", BossPillarSmoke::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_ShieldHit", ShieldHit::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_SelectHome", SelectHome::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoPanda", LogoPanda::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoPlayer", LogoPlayer::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_FreeCam", FreeCam::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_MouseCursor", MouseCursor::Create())))
			return E_FAIL;

		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Boom", Boom::Create())))
			return E_FAIL;
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_LogoShadowCam", LogoShadowCam::Create())))
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
		if (FAILED(engine->LoadModelFromFile(ENUM_CLASS(LevelID::StageBoss), "../bin/resource/models/enemy/boss_eye/boss_eye.model",
			"Model_BossEye")))
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
		if (FAILED(engine->AddPrototype(ENUM_CLASS(LevelID::StageBoss), "Prototype_Object_BossEye", BossEye::Create())))
			return E_FAIL;
	}

	return S_OK;
}

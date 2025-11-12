#pragma once

#ifdef USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "guizmo/ImGuizmo.h"
#endif

namespace Client
{
	constexpr unsigned int WinSizeX = 1600;
	constexpr unsigned int WinSizeY = 900;

	enum class LevelID { Static, Loading, Logo, Stage1, StageBoss, Test, Count };
	enum class ColliderFilter
	{
		Player,
		PlayerProjectile,
		PlayerAttack,
		Enemy,
		EnemyWeakness,
		EnemyAttack,
		EnemyShield,
		Fracture,
		BossArm,
		BossStoneProjectile,
		BossPillar,
		BossArmProjectile,
		Spawner,
		StaticMapObject,
		Item,
		Door,
		Chest,
		Count
	};
	enum class WeaponID
	{
		Foundry,
		ConcealedAmmo,
		PoisionousGhost,
		Cameleon,
		IcySpear,
		Prism,
		Count
	};
	enum class EventID
	{
		ChangeLevel,
		ChangeWeapon,
		PlayerJump,
		PlayerLand,
		PlayerDash,
		PlayerHealthDecrease,
		PlayerHealthIncrease,
		PlayerShieldDecrease,
		PlayerShieldIncrease,
		MaxAmmoChange,
		CurrAmmoChange,
		WeaponReload,
		BossHealthDecrease,
		EnemyHpPanelPositionUpdate,
		EnemyHealthDecrease,
		EnemyDead,
		Count
	};
	enum class SkillID
	{
		Dash,
		Count
	};

	enum class EffectNodeType
	{
		Mesh,
		Sprite,
		Particle,
		Count
	};

}

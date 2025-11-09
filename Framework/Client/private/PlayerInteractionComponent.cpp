#include "pch.h"
#include "PlayerInteractionComponent.h"

//object
#include "PlayerPanel.h"
#include "StatusComponent.h"
#include "Player.h"

PlayerInteractionComponent::PlayerInteractionComponent(Object* owner)
	:Component(owner)
{
}

PlayerInteractionComponent::PlayerInteractionComponent(const PlayerInteractionComponent& prototype)
	:Component(prototype)
{
}

PlayerInteractionComponent* PlayerInteractionComponent::Create(Object* owner)
{
	PlayerInteractionComponent* Instance = new PlayerInteractionComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PlayerInteractionComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT PlayerInteractionComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

void PlayerInteractionComponent::SetPlayer(Player* player)
{
	/*addref 하면 상호참조*/
	m_pPlayer = player;
}

void PlayerInteractionComponent::EquipWeapon(WeaponID id)
{
	m_pPlayer->PickUpWeapon(id);
}

void PlayerInteractionComponent::HealthUp(_uint health)
{
	auto status = m_pPlayer->GetComponent<StatusComponent>();
	status->HealthUp(health);

	PlayerPanel::PLAYER_HEALTH_PARAM param{};
	param.ratio = status->GetHpRatio();
	param.currHealth = status->GetDesc().hp;

	EngineCore::GetInstance()->PublishEvent(ENUM_CLASS(EventID::PlayerHealthIncrease), param);
}

void PlayerInteractionComponent::AddAmmo(_uint ammo)
{
	m_pPlayer->AddAmmo(ammo);
}

void PlayerInteractionComponent::Free()
{
	__super::Free();
}

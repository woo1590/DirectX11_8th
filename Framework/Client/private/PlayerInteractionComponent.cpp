#include "pch.h"
#include "PlayerInteractionComponent.h"

//object
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

void PlayerInteractionComponent::Free()
{
	__super::Free();
}

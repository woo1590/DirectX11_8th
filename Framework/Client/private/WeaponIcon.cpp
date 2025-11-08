#include "pch.h"
#include "WeaponIcon.h"

//component
#include "SpriteComponent.h"

WeaponIcon::WeaponIcon()
	:UIObject()
{
}

WeaponIcon::WeaponIcon(const WeaponIcon& prototype)
	:UIObject(prototype)
{
}

WeaponIcon* WeaponIcon::Create()
{
	WeaponIcon* Instance = new WeaponIcon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT WeaponIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT WeaponIcon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	WEAPON_ICON_DESC* desc = static_cast<WEAPON_ICON_DESC*>(arg);
	_string mtrlTag{};
	switch (desc->weaponID)
	{
	case Client::WeaponID::Foundry:
		mtrlTag = "Mtrl_WeaponIcon_Foundry";
		break;
	case Client::WeaponID::ConcealedAmmo:
		mtrlTag = "Mtrl_WeaponIcon_ConcealedAmmo";
		break;
	case Client::WeaponID::PoisionousGhost:
		mtrlTag = "Mtrl_WeaponIcon_PoisonousGhost";
		break;
	case Client::WeaponID::Cameleon:
		mtrlTag = "Mtrl_WeaponIcon_Cameleon";
		break;
	case Client::WeaponID::IcySpear:
		break;
	case Client::WeaponID::Prism:
		mtrlTag = "Mtrl_WeaponIcon_Prism";
		break;
	default:
		break;
	}

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), mtrlTag);

	_float3 parentScale = m_pParent->GetComponent<TransformComponent>()->GetScale();
	_float3 position{};
	position.x = parentScale.x * 0.5f - parentScale.x * 0.4f;
	position.y = parentScale.y * 0.01f;
	position.z = 1.f;

	m_fSizeX = WinSizeX * 0.1f;
	m_fSizeY = m_fSizeX;

	m_pTransform->SetPosition(position);
	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });
	
	return S_OK;
}

void WeaponIcon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void WeaponIcon::Update(_float dt)
{
	__super::Update(dt);
}

void WeaponIcon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* WeaponIcon::Clone(InitDESC* arg)
{
	WeaponIcon* Instance = new WeaponIcon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void WeaponIcon::Free()
{
	__super::Free();
}

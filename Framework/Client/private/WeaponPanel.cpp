#include "pch.h"
#include "WeaponPanel.h"

//object
#include "CountNumber.h"
#include "Player.h"
#include "WeaponIcon.h"
#include "Cross.h"
#include "WeaponSlot.h"

//component
#include "SpriteComponent.h"

WeaponPanel::WeaponPanel()
	:UIObject()
{
}

WeaponPanel::WeaponPanel(const WeaponPanel& prototype)
	:UIObject(prototype)
{
}

WeaponPanel* WeaponPanel::Create()
{
	WeaponPanel* Instance = new WeaponPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT WeaponPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT WeaponPanel::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	UIOBJECT_DESC desc{};
	desc.parent = nullptr;
	desc.sizeX = 297.f*1.2f;
	desc.sizeY = 94.f*1.2f;
	desc.x = WinSizeX * 0.9f;
	desc.y = WinSizeY * 0.9f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_WeaponPanel");

	m_Childrens.resize(ENUM_CLASS(Parts::Count));

	if (FAILED(CreateChildren()))
		return E_FAIL;

	engine->Subscribe(ENUM_CLASS(EventID::ChangeWeapon), MakeListener(&WeaponPanel::ChangeWeapon));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerJump), MakeListener(&WeaponPanel::OnJump));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerLand), MakeListener(&WeaponPanel::OnLand));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerDash), MakeListener(&WeaponPanel::OnDash));
	engine->Subscribe(ENUM_CLASS(EventID::MaxAmmoChange), MakeListener(&WeaponPanel::ChangeMaxAmmo));
	engine->Subscribe(ENUM_CLASS(EventID::CurrAmmoChange), MakeListener(&WeaponPanel::ChangeCurrAmmo));

	ChangeState(&m_WeaponPanelIdle);

	return S_OK;
}

void WeaponPanel::ChangeWeapon(std::any param)
{
	auto engine = EngineCore::GetInstance();

	/*remove old icon*/
	m_pTransform->RemoveChild(m_Childrens[ENUM_CLASS(Parts::WeaponIcon)]->GetComponent<TransformComponent>());
	Safe_Release(m_Childrens[ENUM_CLASS(Parts::WeaponIcon)]);

	auto eventParam = std::any_cast<Player::CHANGE_WEAPON_EVENT_PARAM>(param);
	/*new weapon icon*/
	{
		WeaponIcon::WEAPON_ICON_DESC iconDesc{};
		iconDesc.parent = this;
		iconDesc.weaponID = eventParam.weaponID;
		iconDesc.priority = 1;

		Object* icon = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponIcon", &iconDesc);
		m_Childrens[ENUM_CLASS(Parts::WeaponIcon)] = static_cast<UIObject*>(icon);
	}

	/*change active slot*/
	{
		for (_uint i = 0; i < 3; ++i)
		{
			WeaponSlot* slot = static_cast<WeaponSlot*>(m_Childrens[i]);
			if (eventParam.slotNum == i)
				slot->SetActive(true);
			else
				slot->SetActive(false);
		}

	}
}

void WeaponPanel::OnJump(std::any param)
{
	if (&m_WeaponPanelIdle == m_CurrState)
		ChangeState(&m_WeaponPanelOnJump);
}

void WeaponPanel::OnLand(std::any param)
{
	if (&m_WeaponPanelIdle == m_CurrState)
		ChangeState(&m_WeaponPanelOnLand);
}

void WeaponPanel::OnDash(std::any param)
{
	ChangeState(&m_WeaponPanelOnDash);
}

void WeaponPanel::ChangeCurrAmmo(std::any param)
{
	_uint currAmmo = std::any_cast<_uint>(param);

	static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_CurrAmmo)])->SetNumber(currAmmo);
	static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_CurrAmmo)])->Bounce();
}

void WeaponPanel::ChangeMaxAmmo(std::any param)
{
	_uint maxAmmo = std::any_cast<_uint>(param);

	static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_MaxAmmo)])->SetNumber(maxAmmo);
}

void WeaponPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void WeaponPanel::Update(_float dt)
{
	__super::Update(dt);
}

void WeaponPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* WeaponPanel::Clone(InitDESC* arg)
{
	WeaponPanel* Instance = new WeaponPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void WeaponPanel::Free()
{
	__super::Free();
}

HRESULT WeaponPanel::CreateChildren()
{
	auto engine = EngineCore::GetInstance();

	/*weapon icon*/
	{
		WeaponIcon::WEAPON_ICON_DESC iconDesc{};
		iconDesc.parent = this;
		iconDesc.weaponID = WeaponID::Foundry;
		iconDesc.priority = 1;

		Object* icon = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponIcon", &iconDesc);
		m_Childrens[ENUM_CLASS(Parts::WeaponIcon)] = static_cast<UIObject*>(icon);
	}

	/*weapon slot1*/
	{
		WeaponSlot::WEAPON_SLOT_DESC slotDesc{};
		slotDesc.parent = this;
		slotDesc.slotNum = 1;
		slotDesc.position = _float3{ -0.5f * m_fSizeX + m_fSizeX * 0.4f,-0.45f * m_fSizeY,0.f };
		slotDesc.priority = 1;

		Object* slot = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponSlot", &slotDesc);
		m_Childrens[ENUM_CLASS(Parts::Slot1)] = static_cast<UIObject*>(slot);
	}
	/*weapon slot2*/
	{
		WeaponSlot::WEAPON_SLOT_DESC slotDesc{};
		slotDesc.parent = this;
		slotDesc.slotNum = 2;
		slotDesc.position = _float3{ -0.5f * m_fSizeX + m_fSizeX * 0.6f,-0.53f * m_fSizeY,0.f };
		slotDesc.priority = 1;
		
		Object* slot = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponSlot", &slotDesc);
		m_Childrens[ENUM_CLASS(Parts::Slot2)] = static_cast<UIObject*>(slot);
	}
	/*weapon slot3*/
	{
		WeaponSlot::WEAPON_SLOT_DESC slotDesc{};
		slotDesc.parent = this;
		slotDesc.slotNum = 3;
		slotDesc.position = _float3{ -0.5f * m_fSizeX + m_fSizeX * 0.8f,-0.6f * m_fSizeY,0.f };
		slotDesc.priority = 1;
		
		Object* slot = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_WeaponSlot", &slotDesc);
		m_Childrens[ENUM_CLASS(Parts::Slot3)] = static_cast<UIObject*>(slot);
	}
	/*num curr ammo*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4{ 1.f,1.f,1.f,1.f };
		numDesc.position = _float3{ m_fSizeX * 0.04f,m_fSizeY * 0.42f,0.f };
		numDesc.scale = _float3{ 60.f * 0.6f,60.f * 0.6f,1.f };
		numDesc.priority = 1;

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_CurrAmmo)] = static_cast<UIObject*>(num);
	}
	/*num cross*/
	{
		Cross::CROSS_DESC crossDesc{};
		crossDesc.parent = this;
		crossDesc.color = _float4{ 1.f,1.f,1.f,1.f };
		crossDesc.position = _float3{ m_fSizeX * 0.18f,m_fSizeY * 0.4f,0.f };
		crossDesc.scale = _float3{ 60.f * 0.4f,60.f * 0.4f,1.f };
		crossDesc.priority = 1;

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cross", &crossDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_Cross)] = static_cast<UIObject*>(num);
	}

	/*num max ammo*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4{ 1.f,1.f,1.f,1.f };
		numDesc.position = _float3{ m_fSizeX * 0.25f,m_fSizeY * 0.4f,0.f };
		numDesc.scale = _float3{ 60.f * 0.4f,60.f * 0.4f,1.f };
		numDesc.priority = 1;
		
		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_MaxAmmo)] = static_cast<UIObject*>(num);
	}

	return S_OK;
}

void WeaponPanel::WeaponPanelOnJump::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y -= 10.f;
}

void WeaponPanel::WeaponPanelOnJump::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);
		t = math::PalabolaCurve(t);

		_float3 currPosition{};
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(currPosition);
	}
}

void WeaponPanel::WeaponPanelOnJump::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<WeaponPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_WeaponPanelIdle);
	}
}

void WeaponPanel::WeaponPanelOnLand::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y += 25.f;
}

void WeaponPanel::WeaponPanelOnLand::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);
		t = math::PalabolaCurve(t);

		_float3 currPosition{};
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(currPosition);
	}
}

void WeaponPanel::WeaponPanelOnLand::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<WeaponPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_WeaponPanelIdle);
	}
}

void WeaponPanel::WeaponPanelOnDash::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.x -= 45.f;
	m_TargetPosition.y += 35.f;
}

void WeaponPanel::WeaponPanelOnDash::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);
		t = math::PalabolaCurve(t);

		_float3 currPosition{};
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(currPosition);
	}
}

void WeaponPanel::WeaponPanelOnDash::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<WeaponPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_WeaponPanelIdle);
	}
}

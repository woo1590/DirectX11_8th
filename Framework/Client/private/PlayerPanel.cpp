#include "pch.h"
#include "PlayerPanel.h"
#include "MaterialInstance.h"

//object
#include "Bar.h"
#include "CountNumber.h"
#include "PlayerIcon.h"
#include "Cross.h"

//component
#include "SpriteComponent.h"

PlayerPanel::PlayerPanel()
	:UIObject()
{
}

PlayerPanel::PlayerPanel(const PlayerPanel& prototype)
	:UIObject(prototype)
{
}

PlayerPanel* PlayerPanel::Create()
{
	PlayerPanel* Instance = new PlayerPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT PlayerPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT PlayerPanel::Initialize(InitDESC* arg)
{
	UIOBJECT_DESC desc{};
	desc.parent = nullptr;
	desc.sizeX = 550.f;
	desc.sizeY = 130.f;
	desc.x = WinSizeX * 0.2f;
	desc.y = WinSizeY * 0.9f;

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	engine->Subscribe(ENUM_CLASS(EventID::PlayerJump), MakeListener(&PlayerPanel::OnJump));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerLand), MakeListener(&PlayerPanel::OnLand));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerHealthDecrease), MakeListener(&PlayerPanel::PlayerHealthDecrease));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerHealthIncrease), MakeListener(&PlayerPanel::PlayerHealthIncrease));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerShieldDecrease), MakeListener(&PlayerPanel::PlayerShieldDecrease));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerShieldIncrease), MakeListener(&PlayerPanel::PlayerShieldIncrease));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerDash), MakeListener(&PlayerPanel::OnDash));

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;
	
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_PlayerPanel");

	m_Childrens.resize(ENUM_CLASS(Parts::Count));

	if (FAILED(CreateChildren()))
		return E_FAIL;

	ChangeState(&m_PlayerPanelIdle);

	return S_OK;
}

void PlayerPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void PlayerPanel::Update(_float dt)
{
	__super::Update(dt);
}

void PlayerPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void PlayerPanel::OnJump(std::any param)
{
	if (&m_PlayerPanelIdle == m_CurrState)
		ChangeState(&m_PlayerPanelOnJump);
}

void PlayerPanel::OnLand(std::any param)
{
	if (&m_PlayerPanelIdle == m_CurrState)
		ChangeState(&m_PlayerPanelOnLand);
}

void PlayerPanel::OnDash(std::any param)
{
	ChangeState(&m_PlayerPanelOnDash);
}

void PlayerPanel::PlayerHealthDecrease(std::any param)
{
	PLAYER_HEALTH_PARAM healthParam = std::any_cast<PLAYER_HEALTH_PARAM>(param);

	_float ratio = healthParam.ratio;
	_uint currHelath = healthParam.currHealth;

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = false;
		barParam.paused = true;
		barParam.pauseDuration = 0.5f;
		barParam.duration = 0.3f;
		barParam.targetRatio = ratio;

		Bar* healthDecrease = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health_Decrease)]);
		healthDecrease->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.1f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health)]);
		health->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.useAlpha = true;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.1f;
		barParam.targetRatio = ratio;

		Bar* healthHurt = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health_Hurt)]);
		healthHurt->MakeChange(barParam);
		healthHurt->InverseAlpha();
	}

	/*num change*/
	{
		static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_Health)])->SetNumber(currHelath);
	}

}

void PlayerPanel::PlayerHealthIncrease(std::any param)
{
	PLAYER_HEALTH_PARAM healthParam = std::any_cast<PLAYER_HEALTH_PARAM>(param);

	_float ratio = healthParam.ratio;
	_uint currHelath = healthParam.currHealth;

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* healthDecrease = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health_Decrease)]);
		healthDecrease->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health)]);
		health->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* healthHurt = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Health_Hurt)]);
		healthHurt->MakeChange(barParam);
	}

	/*num change*/
	{
		static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_Health)])->SetNumber(currHelath);
	}
}

void PlayerPanel::PlayerShieldDecrease(std::any param)
{
	PLAYER_SHIELD_PARAM shieldParam = std::any_cast<PLAYER_SHIELD_PARAM>(param);
	_float ratio = shieldParam.ratio;
	_uint currShield = shieldParam.currShield;

	/*bar change*/
	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = false;
		barParam.paused = true;
		barParam.pauseDuration = 0.5f;
		barParam.duration = 0.3f;
		barParam.targetRatio = ratio;

		Bar* healthDecrease = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Decrease)]);
		healthDecrease->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.1f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield)]);
		health->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.useAlpha = true;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.1f;
		barParam.targetRatio = ratio;

		Bar* healthHurt = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Hurt)]);
		healthHurt->MakeChange(barParam);
		healthHurt->InverseAlpha();
	}

	/*num change*/
	{
		static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_Shield)])->SetNumber(currShield);
	}
}

void PlayerPanel::PlayerShieldIncrease(std::any param)
{
	PLAYER_SHIELD_PARAM shieldParam = std::any_cast<PLAYER_SHIELD_PARAM>(param);
	_float ratio = shieldParam.ratio;
	_uint currShield = shieldParam.currShield;

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* healthDecrease = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Decrease)]);
		healthDecrease->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* health = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield)]);
		health->MakeChange(barParam);
	}

	{
		Bar::BAR_CHANGE_PARAM barParam{};
		barParam.immediate = true;
		barParam.paused = false;
		barParam.pauseDuration = 0.f;
		barParam.duration = 0.f;
		barParam.targetRatio = ratio;

		Bar* healthHurt = static_cast<Bar*>(m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Hurt)]);
		healthHurt->MakeChange(barParam);
	}

	/*num change*/
	{
		static_cast<CountNumber*>(m_Childrens[ENUM_CLASS(Parts::Number_Shield)])->SetNumber(currShield);
	}
}

Object* PlayerPanel::Clone(InitDESC* arg)
{
	PlayerPanel* Instance = new PlayerPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void PlayerPanel::Free()
{
	__super::Free();

}

HRESULT PlayerPanel::CreateChildren()
{
	auto engine = EngineCore::GetInstance();

	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();

	/*player icon*/
	{
		PlayerIcon::PLAYER_ICON_DESC iconDesc{};
		iconDesc.parent = this;
		iconDesc.priority = 1;

		Object* icon = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_PlayerIcon", &iconDesc);
		m_Childrens[ENUM_CLASS(Parts::PlayerIcon)] = static_cast<UIObject*>(icon);
	}

	/*bar shield background1*/
	{
		Bar::BAR_DESC shieldBackDesc{};
		shieldBackDesc.parent = this;
		shieldBackDesc.priority = 1;
		shieldBackDesc.mtrlTag = "Mtrl_Bar_Background";
		shieldBackDesc.position = _float3{ scale.x * 0.15f,scale.y * 0.15f,0.f };
		shieldBackDesc.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar =engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &shieldBackDesc);
		m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Background)] = static_cast<UIObject*>(bar);

	}

	/*bar shield decrease*/
	{
		Bar::BAR_DESC shieldDecrase{};
		shieldDecrase.parent = this;
		shieldDecrase.priority = 1;
		shieldDecrase.mtrlTag = "Mtrl_Bar_Background2";
		shieldDecrase.position = _float3{ scale.x * 0.15f,scale.y * 0.15f,0.f };
		shieldDecrase.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &shieldDecrase);
		m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Decrease)] = static_cast<UIObject*>(bar);

	}
	
	/*bar shield*/
	{
		Bar::BAR_DESC shield{};
		shield.parent = this;
		shield.priority = 1;
		shield.mtrlTag = "Mtrl_Bar_Shield";
		shield.position = _float3{ scale.x * 0.15f,scale.y * 0.15f,0.f };
		shield.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &shield);
		m_Childrens[ENUM_CLASS(Parts::Bar_Shield)] = static_cast<UIObject*>(bar);

	}

	/*bar shield hurt*/
	{
		Bar::BAR_DESC healthHurt{};
		healthHurt.parent = this;
		healthHurt.priority = 1;
		healthHurt.mtrlTag = "Mtrl_Bar_Hurt";
		healthHurt.position = _float3{ scale.x * 0.15f,scale.y * 0.15f,0.f };
		healthHurt.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &healthHurt);
		m_Childrens[ENUM_CLASS(Parts::Bar_Shield_Hurt)] = static_cast<UIObject*>(bar);

		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	}


	/*bar health background*/
	{
		Bar::BAR_DESC healthBack{};
		healthBack.parent = this;
		healthBack.priority = 1;
		healthBack.mtrlTag = "Mtrl_Bar_Background";
		healthBack.position = _float3{ scale.x * 0.12f,0.f,0.f };
		healthBack.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &healthBack);
		m_Childrens[ENUM_CLASS(Parts::Bar_Health_Background)] = static_cast<UIObject*>(bar);
	}
	
	/*bar health decrease*/
	{
		Bar::BAR_DESC healthDecrease{};
		healthDecrease.parent = this;
		healthDecrease.priority = 1;
		healthDecrease.mtrlTag = "Mtrl_Bar_Background2";
		healthDecrease.position = _float3{ scale.x * 0.12f,0.f,0.f };
		healthDecrease.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &healthDecrease);
		m_Childrens[ENUM_CLASS(Parts::Bar_Health_Decrease)] = static_cast<UIObject*>(bar);
	}
	
	/*bar health*/
	{
		Bar::BAR_DESC health{};
		health.parent = this;
		health.priority = 1;
		health.mtrlTag = "Mtrl_Bar_HP";
		health.position = _float3{ scale.x * 0.12f,0.f,0.f };
		health.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &health);
		m_Childrens[ENUM_CLASS(Parts::Bar_Health)] = static_cast<UIObject*>(bar);
	}

	/*bar health hurt*/
	{
		Bar::BAR_DESC healthHurt{};
		healthHurt.parent = this;
		healthHurt.priority = 1;
		healthHurt.mtrlTag = "Mtrl_Bar_Hurt";
		healthHurt.position = _float3{ scale.x * 0.12f,0.f,0.f };
		healthHurt.scale = _float3{ 286.f * 0.8f,41.f * 0.8f,1.f };

		Object* bar = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Bar", &healthHurt);
		m_Childrens[ENUM_CLASS(Parts::Bar_Health_Hurt)] = static_cast<UIObject*>(bar);

		bar->GetComponent<SpriteComponent>()->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	}

	/*number shield*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4{ 0.094f, 0.84f, 0.94f, 1.f };
		numDesc.priority = 1;
		numDesc.position = _float3{ scale.x * 0.37f,scale.y * 0.17f,0.f };
		numDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_Shield)] = static_cast<UIObject*>(num);
	}

	/*number shield cross*/
	{
		Cross::CROSS_DESC crossDesc{};
		crossDesc.parent = this;
		crossDesc.color = _float4{ 0.094f, 0.84f, 0.94f, 1.f };
		crossDesc.priority = 1;
		crossDesc.position = _float3{ scale.x * 0.41f,scale.y * 0.17f,0.f };
		crossDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cross", &crossDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_Shield_Cross)] = static_cast<UIObject*>(num);
	}

	/*number shield max*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4{ 0.094f, 0.84f, 0.94f, 1.f };
		numDesc.priority = 1;
		numDesc.position = _float3{ scale.x * 0.45f,scale.y * 0.17f,0.f };
		numDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_MaxShield)] = static_cast<UIObject*>(num);
	}

	/*number helath*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4(0.56f, 0.92f, 0.32f, 1.f);
		numDesc.priority = 1;
		numDesc.position = _float3{ scale.x * 0.34f,scale.y * 0.05f,0.f };
		numDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_Health)] = static_cast<UIObject*>(num);
	}

	/*number health cross*/
	{
		Cross::CROSS_DESC crossDesc{};
		crossDesc.parent = this;
		crossDesc.color = _float4(0.56f, 0.92f, 0.32f, 1.f);
		crossDesc.priority = 1;
		crossDesc.position = _float3{ scale.x * 0.39f,scale.y * 0.05f,0.f };
		crossDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_Cross", &crossDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_Health_Cross)] = static_cast<UIObject*>(num);
	}

	/*number health max*/
	{
		CountNumber::COUNT_NUMBER_DESC numDesc{};
		numDesc.parent = this;
		numDesc.color = _float4(0.56f, 0.92f, 0.32f, 1.f);
		numDesc.priority = 1;
		numDesc.position = _float3{ scale.x * 0.43f,scale.y * 0.05f,0.f };
		numDesc.scale = _float3{ 60.f * 0.2f,60.f * 0.2f,1.f };

		Object* num = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_CountNumber", &numDesc);
		m_Childrens[ENUM_CLASS(Parts::Number_MaxHealth)] = static_cast<UIObject*>(num);
	}

	return S_OK;
}

void PlayerPanel::PlayerPanelOnJump::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y -= 10.f;
}

void PlayerPanel::PlayerPanelOnJump::Update(Object* object, _float dt)
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

void PlayerPanel::PlayerPanelOnJump::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<PlayerPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_PlayerPanelIdle);
	}
}

void PlayerPanel::PlayerPanelOnLand::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y += 25.f;
}

void PlayerPanel::PlayerPanelOnLand::Update(Object* object, _float dt)
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

void PlayerPanel::PlayerPanelOnLand::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<PlayerPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_PlayerPanelIdle);
	}
}

void PlayerPanel::PlayerPanelOnDash::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.x += 45.f;
	m_TargetPosition.y += 35.f;
}

void PlayerPanel::PlayerPanelOnDash::Update(Object* object, _float dt)
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

void PlayerPanel::PlayerPanelOnDash::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<PlayerPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_PlayerPanelIdle);
	}
}

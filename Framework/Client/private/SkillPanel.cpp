#include "pch.h"
#include "SkillPanel.h"

//object
#include "DashIcon.h"

//component
#include "SpriteComponent.h"

SkillPanel::SkillPanel()
	:UIObject()
{
}

SkillPanel::SkillPanel(const SkillPanel& prototype)
	:UIObject(prototype)
{
}

SkillPanel* SkillPanel::Create()
{
	SkillPanel* Instance = new SkillPanel();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT SkillPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT SkillPanel::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	SKILL_PANEL_DESC* desc = static_cast<SKILL_PANEL_DESC*>(arg);

	auto engine = EngineCore::GetInstance();

	engine->Subscribe(ENUM_CLASS(EventID::PlayerJump), MakeListener(&SkillPanel::OnJump));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerLand), MakeListener(&SkillPanel::OnLand));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerDash), MakeListener(&SkillPanel::OnDash));

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_SkillPanel");

	m_fSizeX = 100.f;
	m_fSizeY = 86.f;
	_float3 position{ WinSizeX * 0.5f - WinSizeX * 0.23f, WinSizeY * -0.5f + WinSizeY * 0.12,0.f };
	position.x -= desc->numPanel * m_fSizeX * 0.5f;

	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });
	m_pTransform->SetPosition(position);

	/*create slot*/
	m_Childrens.resize(ENUM_CLASS(Parts::Count));

	switch (desc->skillID)
	{
	case Client::SkillID::Dash:
	{
		DashIcon::DASH_ICON_DESC dashDesc{};
		dashDesc.parent = this;
		dashDesc.priority = 1;
		Object* dash = engine->ClonePrototype(ENUM_CLASS(LevelID::Static), "Prototype_Object_DashIcon", &dashDesc);
		m_Childrens[ENUM_CLASS(Parts::Slot)] = static_cast<UIObject*>(dash);
	}break;
	default:
		break;
	}

	ChangeState(&m_SkillPanelIdle);

	return S_OK;
}

void SkillPanel::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void SkillPanel::Update(_float dt)
{
	__super::Update(dt);
}

void SkillPanel::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void SkillPanel::OnJump(std::any param)
{
	if (&m_SkillPanelIdle == m_CurrState)
		ChangeState(&m_SkillPanelOnJump);
}

void SkillPanel::OnLand(std::any param)
{
	if (&m_SkillPanelIdle == m_CurrState)
		ChangeState(&m_SkillPanelOnLand);
}

void SkillPanel::OnDash(std::any param)
{
	ChangeState(&m_SkillPanelOnDash);
}

Object* SkillPanel::Clone(InitDESC* arg)
{
	SkillPanel* Instance = new SkillPanel(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void SkillPanel::Free()
{
	__super::Free();

}

HRESULT SkillPanel::CreateChildren()
{
	return S_OK;
}

void SkillPanel::SkillPanelOnJump::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y -= 10.f;
}

void SkillPanel::SkillPanelOnJump::Update(Object* object, _float dt)
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

void SkillPanel::SkillPanelOnJump::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<SkillPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_SkillPanelIdle);
	}
}

void SkillPanel::SkillPanelOnLand::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.y += 25.f;
}

void SkillPanel::SkillPanelOnLand::Update(Object* object, _float dt)
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

void SkillPanel::SkillPanelOnLand::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<SkillPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_SkillPanelIdle);
	}
}

void SkillPanel::SkillPanelOnDash::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;
	m_TargetPosition.x -= 45.f;
	m_TargetPosition.y += 35.f;
}

void SkillPanel::SkillPanelOnDash::Update(Object* object, _float dt)
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

void SkillPanel::SkillPanelOnDash::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto panel = static_cast<SkillPanel*>(object);
		panel->GetComponent<TransformComponent>()->SetPosition(m_StartPosition);
		panel->ChangeState(&panel->m_SkillPanelIdle);
	}
}

#include "pch.h"
#include "DashIcon.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

DashIcon::DashIcon()
	:UIObject()
{
}

DashIcon::DashIcon(const DashIcon& prototype)
	:UIObject(prototype)
{
}

DashIcon* DashIcon::Create()
{
	DashIcon* Instance = new DashIcon();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DashIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT DashIcon::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Dash0");

	m_fSizeX = 48.f;
	m_fSizeY = m_fSizeX;
	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });

	engine->Subscribe(ENUM_CLASS(EventID::PlayerDash), MakeListener(&DashIcon::DashDeactive));

	return S_OK;
}

void DashIcon::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DashIcon::Update(_float dt)
{
	__super::Update(dt);

	if (!m_IsActive)
	{
		m_fElapsedTime += dt;

		if (m_fElapsedTime >= m_fDashCoolDownDuration)
		{
			auto sprite = GetComponent<SpriteComponent>();
			sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Dash0");
			m_IsActive = true;
		}
	}
}

void DashIcon::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* DashIcon::Clone(InitDESC* arg)
{
	DashIcon* Instance = new DashIcon(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DashIcon::DashDeactive(std::any dashCoolDown)
{
	auto sprite = GetComponent<SpriteComponent>();
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_Dash1");

	m_IsActive = false;
	m_fDashCoolDownDuration = std::any_cast<_float>(dashCoolDown);
	m_fElapsedTime = 0.f;
}


void DashIcon::Free()
{
	__super::Free();
}

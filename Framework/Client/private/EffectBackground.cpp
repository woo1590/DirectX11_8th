#include "pch.h"
#include "EffectBackground.h"
#include "MaterialInstance.h"

// component
#include "SpriteComponent.h"

EffectBackground::EffectBackground()
	:UIObject()
{
}

EffectBackground::EffectBackground(const EffectBackground& prototype)
	:UIObject(prototype)
{
}

EffectBackground* EffectBackground::Create()
{
	EffectBackground* Instance = new EffectBackground();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EffectBackground::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT EffectBackground::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
	desc.sizeX = WinSizeX;
	desc.sizeY = WinSizeY;
	desc.x = WinSizeX * 0.5f;
	desc.y = WinSizeY * 0.5f;

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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_EffectBackground");

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("BackgroundEffect_Pass");
	mtrlInstance->SetFloat("g_CustomAlpha", 0.f);

	engine->Subscribe(ENUM_CLASS(EventID::PlayerHealthDecrease), MakeListener(&EffectBackground::OnHurt));
	engine->Subscribe(ENUM_CLASS(EventID::PlayerHealthIncrease), MakeListener(&EffectBackground::OnHeal));

	return S_OK;
}

void EffectBackground::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EffectBackground::Update(_float dt)
{
	__super::Update(dt);

	if (m_IsActive)
	{
		m_fElapsedTime += dt;
		if (m_fElapsedTime >= m_fDuration)
		{
			m_IsActive = false;
			return;
		}

		_float t = m_fElapsedTime / m_fDuration;
		t = math::SmoothStep(t);

		t = 1.f - t;

		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();
		mtrlInstance->SetFloat("g_CustomAlpha", t);
	}
}

void EffectBackground::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void EffectBackground::OnHurt(std::any param)
{
	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();
	mtrlInstance->SetFloat4("g_MaskColor", _float4{ 0.82f,0.01f,0.11f,1.f });
	
	m_IsActive = true;
	m_fElapsedTime = 0.f;
	m_fDuration = 1.2f;
}

void EffectBackground::OnHeal(std::any param)
{
	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();
	mtrlInstance->SetFloat4("g_MaskColor", _float4{ 0.443f, 0.82f, 0.482f,1.f });

	m_IsActive = true;
	m_fElapsedTime = 0.f;
	m_fDuration = 0.8f;
}

Object* EffectBackground::Clone(InitDESC* arg)
{
	EffectBackground* Instance = new EffectBackground(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EffectBackground::Free()
{
	__super::Free();

}
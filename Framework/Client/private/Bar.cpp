#include "pch.h"
#include "Bar.h"
#include "MaterialInstance.h"

//object

//component
#include "SpriteComponent.h"

Bar::Bar()
	:UIObject()
{
}

Bar::Bar(const Bar& prototype)
	:UIObject(prototype)
{
}

Bar* Bar::Create()
{
	Bar* Instance = new Bar();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT Bar::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	BAR_DESC* desc = static_cast<BAR_DESC*>(arg);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), desc->mtrlTag);

	auto mtrlInstance = sprite->GetMaterialInstance();
	mtrlInstance->SetPass("PlayerBar_Pass");
	mtrlInstance->SetFloat("g_Ratio", 1.f);
	mtrlInstance->SetFloat("g_CustomAlpha", 1.f);

	m_fCurrRatio = 1.f;
	m_fTargetRatio = 1.f;
	m_fElapsedTime = 0.f;
	m_fDuration = 0.f;

	m_OriginScale = desc->scale;

	return S_OK;
}

void Bar::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Bar::Update(_float dt)
{
	__super::Update(dt);

	if (m_IsUsedAlpha)
	{
		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

		m_fAlphaElapsedTime += dt;

		if (m_fAlphaElapsedTime<m_fAlphaDuration)
		{
			_float t = m_fAlphaElapsedTime / m_fAlphaDuration;
			t = math::SmoothStep(t);
			t = 1.f - t;

			mtrlInstance->SetFloat("g_CustomAlpha", t);
		}
		else
		{
			m_IsUsedAlpha = false;
			m_fAlphaElapsedTime = 0.f;
			mtrlInstance->SetFloat("g_CustomAlpha", 0.f);
		}
	}

	if (m_IsChanged)
	{
		auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();

		if (m_IsPaused)
		{
			m_fPauseElapsedTime += dt;
			if (m_fPauseElapsedTime >= m_fPauseDuration)
			{
				m_IsPaused = false;
			}
		}
		else
		{
			m_fElapsedTime += dt;

			if (m_IsImmediateChange)
			{
				if (m_fElapsedTime >= m_fDuration)
				{
					m_fCurrRatio = m_fTargetRatio;
					m_IsChanged = false;

					mtrlInstance->SetFloat("g_Ratio", m_fCurrRatio);
				}
			}
			else
			{
				if (m_fElapsedTime >= m_fDuration)
				{
					m_fCurrRatio = m_fTargetRatio;
					mtrlInstance->SetFloat("g_Ratio", m_fCurrRatio);
					m_IsChanged = false;

					return;
				}

				_float t = m_fElapsedTime / m_fDuration;
				t = math::EaseOutQuint(t);

				_float currRatio = math::Lerp(m_fCurrRatio, m_fTargetRatio, t);

				mtrlInstance->SetFloat("g_Ratio", currRatio);
			}
		}
	}
}

void Bar::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Bar::MakeChange(BAR_CHANGE_PARAM param)
{
	m_IsChanged = true;
	m_IsUsedAlpha = param.useAlpha;
	m_IsPaused = param.paused;
	m_IsImmediateChange = param.immediate;

	m_fTargetRatio = param.targetRatio;
	m_fPauseDuration = param.pauseDuration;
	m_fDuration = param.duration;

	m_fElapsedTime = 0.f;
	m_fPauseElapsedTime = 0.f;
}

void Bar::InverseAlpha()
{
	m_fAlphaElapsedTime = 0.f;
	m_IsUsedAlpha = true;
}

Object* Bar::Clone(InitDESC* arg)
{
	Bar* Instance = new Bar(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Bar::Free()
{
	__super::Free();

}

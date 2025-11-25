#include "pch.h"
#include "VictoryText.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

VictoryText::VictoryText()
	:UIObject()
{
}

VictoryText::VictoryText(const VictoryText& prototype)
	:UIObject(prototype)
{
}

VictoryText* VictoryText::Create()
{
	VictoryText* Instance = new VictoryText();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VictoryText::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	return S_OK;
}

HRESULT VictoryText::Initialize(InitDESC* arg)
{
	UIObject::UIOBJECT_DESC desc{};
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
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_VictoryText");
	sprite->GetMaterialInstance()->SetPass("CustomAlpha_Pass");
	sprite->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.3f);

	m_pWhiteText = SpriteComponent::Create(this);
	m_pWhiteText->Initialize(&spriteDesc);
	m_pWhiteText->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pWhiteText->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_VictoryTextWhite");
	m_pWhiteText->GetMaterialInstance()->SetPass("CustomAlpha_Pass");
	m_pWhiteText->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);

	/*rock*/
	SpriteComponent::SPRITE_DESC rockDesc{};
	rockDesc.isAnimated = false;
	rockDesc.iMaxFrameIndex = 2;
	m_pRock = SpriteComponent::Create(this);
	m_pRock->Initialize(&rockDesc);
	m_pRock->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_pRock->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_VictoryRock");

	m_pRockTransform = TransformComponent::Create(this);

	ChangeState(&m_VictoryTextAppear);

	return S_OK;
}

void VictoryText::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void VictoryText::Update(_float dt)
{
	__super::Update(dt);

	if (m_pRing)
		m_pRing->Update(dt);
}

void VictoryText::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT VictoryText::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (m_pRock)
		m_pRock->ExtractRenderProxy(m_pRockTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	__super::ExtractRenderProxies(proxies);

	if (m_pWhiteText)
		m_pWhiteText->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	if (m_pRing)
		m_pRing->ExtractRenderProxy(m_pRingTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);

	return S_OK;
}

Object* VictoryText::Clone(InitDESC* arg)
{
	VictoryText* Instance = new VictoryText(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void VictoryText::Free()
{
	__super::Free();

	Safe_Release(m_pWhiteText);
	Safe_Release(m_pRing);
	Safe_Release(m_pRingTransform);
	Safe_Release(m_pRock);
	Safe_Release(m_pRockTransform);
}

void VictoryText::VictoryTextAppear::Enter(Object* object)
{
	m_fElapsedTime = 0.f;

	m_StartScale = _float3{ 700.f * 3.f,350.f * 3.f,1.f };
	m_EndScale = _float3{ 700.f * 0.8f,350.f * 0.8f,1.f };

	m_RockStartScale = _float3{ 342.f * 0.1f,311.f * 0.1f,1.f };
	m_RockEndScale = _float3{ 342.f * 1.2f,311.f * 1.2f,1.f };
}

void VictoryText::VictoryTextAppear::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
	if (m_fElapsedTime < m_fDuration)
	{
		auto victory = static_cast<VictoryText*>(object);
		auto transform = victory->GetComponent<TransformComponent>();
		auto sprite = victory->GetComponent<SpriteComponent>();

		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		if (t >= 0.5f)
		{
			m_fWhiteElapsedTime += dt;
			if (m_fWhiteElapsedTime >= m_fWhiteDuration)
				victory->m_pWhiteText->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
			else
			{
				_float whiteProgress = m_fWhiteElapsedTime / m_fWhiteDuration;
				whiteProgress = math::PalabolaCurve(whiteProgress);
				whiteProgress = (std::min)(whiteProgress, 0.7f);
				victory->m_pWhiteText->GetMaterialInstance()->SetFloat("g_CustomAlpha", whiteProgress);
			}
		}

		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_EndScale), t));
		transform->SetScale(currScale);
		sprite->GetMaterialInstance()->SetFloat("g_CustomAlpha", t);

		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_RockStartScale), XMLoadFloat3(&m_RockEndScale), t));
		victory->m_pRockTransform->SetScale(currScale);
	}
}

void VictoryText::VictoryTextAppear::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto victory = static_cast<VictoryText*>(object);
		victory->GetComponent<TransformComponent>()->SetScale(m_EndScale);
		victory->ChangeState(&victory->m_VictoryTextIdle);
	}
}

void VictoryText::VictoryTextIdle::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartScale = _float3{ 700.f * 0.9f,350.f * 0.9f,1.f };
	m_EndScale = _float3{ 700.f,350.f,1.f };

	m_RingStartScale = _float3{ WinSizeX * 0.5f,WinSizeX * 0.5f,1.f };
	m_RingEndScale = _float3{ WinSizeX * 1.6f,WinSizeX * 1.6f,1.f };

	/*rock*/
	m_RockStartScale = _float3{ 342.f * 0.8f,311.f * 0.8f,1.f };
	m_RockEndScale = _float3{ 342.f,311.f,1.f };

	/*ring*/
	auto victory = static_cast<VictoryText*>(object);

	SpriteComponent::SPRITE_DESC ringDesc{};
	ringDesc.fSpeed = 30.f;
	ringDesc.iMaxFrameIndex = 16;
	ringDesc.isAnimated = true;
	ringDesc.isRepeat = false;
	victory->m_pRing = SpriteComponent::Create(object);
	victory->m_pRing->Initialize(&ringDesc);
	victory->m_pRing->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	victory->m_pRing->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_VictoryRing");
	victory->m_pRing->GetMaterialInstance()->SetPass("CustomAlpha_Pass");
	victory->m_pRing->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);

	victory->m_pRingTransform = TransformComponent::Create(object);
}

void VictoryText::VictoryTextIdle::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		auto victory = static_cast<VictoryText*>(object);
		auto transform = object->GetComponent<TransformComponent>();
		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_EndScale), t));
		transform->SetScale(currScale);

		/*ring*/
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_RingStartScale), XMLoadFloat3(&m_RingEndScale), math::EaseOutQuint(t)));
		victory->m_pRingTransform->SetScale(currScale);
		victory->m_pRing->GetMaterialInstance()->SetFloat("g_CustomAlpha", std::clamp(t, 0.6f, 1.f));

		/*rock*/
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_RockStartScale), XMLoadFloat3(&m_RockEndScale), t));
		victory->m_pRockTransform->SetScale(currScale);
	}
}

void VictoryText::VictoryTextIdle::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto victory = static_cast<VictoryText*>(object);
		victory->ChangeState(&victory->m_VictoryTextDisappear);

		Safe_Release(victory->m_pRing);
		Safe_Release(victory->m_pRingTransform);
	}
}

void VictoryText::VictoryTextDisappear::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
	m_StartScale = _float3{ 700.f,350.f,1.f };
	m_EndScale = _float3{ 700.f * 0.05f,350.f * 0.05f,1.f };

	m_RockStartScale = _float3{ 342.f,311.f,1.f };
	m_RockEndScale = _float3{ 342.f * 3.5f,311.f * 3.5f,1.f };
}

void VictoryText::VictoryTextDisappear::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;
	if (m_fElapsedTime < m_fDuration)
	{
		auto victory = static_cast<VictoryText*>(object);

		_float t = m_fElapsedTime / m_fDuration;

		auto transform = object->GetComponent<TransformComponent>();
		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_EndScale), t = math::EaseInQuint(t)));
		transform->SetScale(currScale);

		if (t >= 0.4f)
			victory->m_pRock->SetFrameIndex(1);
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_RockStartScale), XMLoadFloat3(&m_RockEndScale), t));
		victory->m_pRockTransform->SetScale(currScale);
	}
}

void VictoryText::VictoryTextDisappear::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
		object->SetDead();
}

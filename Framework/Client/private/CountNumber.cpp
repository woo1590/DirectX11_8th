#include "pch.h"
#include "CountNumber.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

CountNumber::CountNumber()
	:UIObject()
{
}

CountNumber::CountNumber(const CountNumber& prototype)
	:UIObject(prototype)
{
}

CountNumber* CountNumber::Create()
{
	CountNumber* Instance = new CountNumber();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CountNumber::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CountNumber::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	CountNumber::COUNT_NUMBER_DESC* desc = static_cast<COUNT_NUMBER_DESC*>(arg);

	m_Numbers.resize(ENUM_CLASS(Digit::Count));
	
	/*sprites*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 9;
	spriteDesc.isRepeat = false;
	spriteDesc.fSpeed = 0.f;

	auto spriteHundread = SpriteComponent::Create(this);
	auto spriteTen = SpriteComponent::Create(this);
	auto spriteUnit = SpriteComponent::Create(this);

	spriteHundread->Initialize(&spriteDesc);
	spriteTen->Initialize(&spriteDesc);
	spriteUnit->Initialize(&spriteDesc);

	auto mtrlHundread = spriteHundread->GetMaterialInstance();
	auto mtrlTens = spriteTen->GetMaterialInstance();
	auto mtrlUnits = spriteUnit->GetMaterialInstance();

	spriteHundread->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	spriteTen->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	spriteUnit->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");

	spriteHundread->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumCount");
	spriteTen->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumCount");
	spriteUnit->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumCount");

	mtrlHundread->SetPass("CountNumber_Pass");
	mtrlTens->SetPass("CountNumber_Pass");
	mtrlUnits->SetPass("CountNumber_Pass");

	mtrlHundread->SetFloat4("g_Color", desc->color);
	mtrlTens->SetFloat4("g_Color", desc->color);
	mtrlUnits->SetFloat4("g_Color", desc->color);

	m_Numbers[ENUM_CLASS(Digit::Hundreads)] = spriteHundread;
	m_Numbers[ENUM_CLASS(Digit::Tens)] = spriteTen;
	m_Numbers[ENUM_CLASS(Digit::Units)] = spriteUnit;

	m_pTransform->SetPosition(desc->position);
	m_pTransform->SetScale(desc->scale);

	m_OriginScale = m_pTransform->GetScale();
	XMStoreFloat3(&m_TargetScale, XMLoadFloat3(&m_OriginScale) * 1.3f);
	m_TargetScale.z = 1.f;

	SetNumber(100);

	return S_OK;
}

void CountNumber::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void CountNumber::Update(_float dt)
{
	__super::Update(dt);

	if (m_UseBounce)
	{
		m_fElapsedTime += dt;
		if (m_fElapsedTime >= m_fDuration)
		{
			m_UseBounce = false;
			m_pTransform->SetScale(m_OriginScale);
			return;
		}

		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);
		t = math::PalabolaCurve(t);

		_float3 currScale{};
		XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_OriginScale), XMLoadFloat3(&m_TargetScale), t));

		m_pTransform->SetScale(currScale);
	}
}

void CountNumber::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT CountNumber::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();

	for (_uint i = 0; i < ENUM_CLASS(Digit::Count); ++i)
	{
		_float3 numPosition = position;
		numPosition.x += (i - 1.f) * scale.x * 0.8f;
		m_pTransform->SetPosition(numPosition);

		m_Numbers[i]->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	}

	m_pTransform->SetPosition(position);

	return S_OK;
}

void CountNumber::SetActive(_bool active)
{
	if (!active)
	{
		m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
		m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
		m_Numbers[ENUM_CLASS(Digit::Units)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	}
}

void CountNumber::SetNumber(_uint number)
{
	m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);
	m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);
	m_Numbers[ENUM_CLASS(Digit::Units)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);

	if (number < 100)
		m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	if (number < 10)
		m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);

	_uint hundread = number / 100;
	_uint ten = (number - hundread * 100) / 10;
	_uint unit = (number - (hundread * 100) - (ten * 10));

	m_Numbers[ENUM_CLASS(Digit::Hundreads)]->SetFrameIndex(hundread);
	m_Numbers[ENUM_CLASS(Digit::Tens)]->SetFrameIndex(ten);
	m_Numbers[ENUM_CLASS(Digit::Units)]->SetFrameIndex(unit);
}

void CountNumber::Bounce()
{
	m_UseBounce = true;
	m_fElapsedTime = 0.f;
}

Object* CountNumber::Clone(InitDESC* arg)
{
	CountNumber* Instance = new CountNumber(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void CountNumber::Free()
{
	__super::Free();

	for (auto& number : m_Numbers)
		Safe_Release(number);
	m_Numbers.clear();
}

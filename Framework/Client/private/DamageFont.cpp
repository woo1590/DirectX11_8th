#include "pch.h"
#include "DamageFont.h"
#include "Random.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

DamageFont::DamageFont()
	:Object()
{
}

DamageFont::DamageFont(const DamageFont& prototype)
	:Object(prototype)
{
}

DamageFont* DamageFont::Create()
{
	DamageFont* Instance = new DamageFont();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DamageFont::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();

	m_strInstanceTag = "DamageFont";
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT DamageFont::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto random = EngineCore::GetInstance()->GetRandom();

	DAMAGE_FONT_DESC* desc = static_cast<DAMAGE_FONT_DESC*>(arg);

	m_Numbers.resize(ENUM_CLASS(Digit::Count));

	/*sprites*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.useBillboard = true;
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 9;
	spriteDesc.isRepeat = false;
	spriteDesc.fSpeed = 0.f;

	auto spriteThousand = SpriteComponent::Create(this);
	auto spriteHundread = SpriteComponent::Create(this);
	auto spriteTen = SpriteComponent::Create(this);
	auto spriteUnit = SpriteComponent::Create(this);

	spriteThousand->Initialize(&spriteDesc);
	spriteHundread->Initialize(&spriteDesc);
	spriteTen->Initialize(&spriteDesc);
	spriteUnit->Initialize(&spriteDesc);

	auto mtrlThousand = spriteThousand->GetMaterialInstance();
	auto mtrlHundread = spriteHundread->GetMaterialInstance();
	auto mtrlTens = spriteTen->GetMaterialInstance();
	auto mtrlUnits = spriteUnit->GetMaterialInstance();

	spriteThousand->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	spriteHundread->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	spriteTen->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	spriteUnit->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");

	spriteThousand->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumHit");
	spriteHundread->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumHit");
	spriteTen->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumHit");
	spriteUnit->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_NumHit");

	mtrlThousand->SetPass("DamageFont_Pass");
	mtrlHundread->SetPass("DamageFont_Pass");
	mtrlTens->SetPass("DamageFont_Pass");
	mtrlUnits->SetPass("DamageFont_Pass");

	mtrlThousand->SetFloat4("g_Color", desc->color);
	mtrlHundread->SetFloat4("g_Color", desc->color);
	mtrlTens->SetFloat4("g_Color", desc->color);
	mtrlUnits->SetFloat4("g_Color", desc->color);

	mtrlThousand->SetFloat("g_FontSize", desc->fontSize);
	mtrlHundread->SetFloat("g_FontSize", desc->fontSize);
	mtrlTens->SetFloat("g_FontSize", desc->fontSize);
	mtrlUnits->SetFloat("g_FontSize", desc->fontSize);

	mtrlThousand->SetFloat("g_Digit", 0.f);
	mtrlHundread->SetFloat("g_Digit",1.f);
	mtrlTens->SetFloat("g_Digit", 2.f);
	mtrlUnits->SetFloat("g_Digit", 3.f);

	m_Numbers[ENUM_CLASS(Digit::Thousands)] = spriteThousand;
	m_Numbers[ENUM_CLASS(Digit::Hundreads)] = spriteHundread;
	m_Numbers[ENUM_CLASS(Digit::Tens)] = spriteTen;
	m_Numbers[ENUM_CLASS(Digit::Units)] = spriteUnit;

	_float3 position = desc->position;
	position.x += random->get<_float>(-4.f, 4.f);
	position.y += random->get<_float>(-4.f, 4.f);
	position.z += random->get<_float>(-4.f, 4.f);

	m_pTransform->SetPosition(position);

	m_fFontSize = desc->fontSize;
	ChangeState(&m_DamageFontSpawn);
	SetNumber(desc->number);

	return S_OK;

}

void DamageFont::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DamageFont::Update(_float dt)
{
	__super::Update(dt);
}

void DamageFont::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT DamageFont::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	_float3 position = m_pTransform->GetPosition();
	_float3 scale = m_pTransform->GetScale();
	_vector right = m_pTransform->GetRightV();

	for (_uint i = 0; i < ENUM_CLASS(Digit::Count); ++i)
	{
		m_Numbers[i]->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::UI)]);
	}

	m_pTransform->SetPosition(position);

	return S_OK;
}

void DamageFont::SetNumber(_uint number)
{
	m_Numbers[ENUM_CLASS(Digit::Thousands)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);
	m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);
	m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);
	m_Numbers[ENUM_CLASS(Digit::Units)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);

	if (number < 1000)
		m_Numbers[ENUM_CLASS(Digit::Thousands)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	if (number < 100)
		m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	if (number < 10)
		m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);

	_uint thousand = number / 1000;
	_uint hundread = (number - thousand * 1000) / 100;
	_uint ten = (number - (thousand * 1000) - (hundread * 100)) / 10;
	_uint unit = (number - (thousand * 1000) - (hundread * 100) - (ten * 10));

	m_Numbers[ENUM_CLASS(Digit::Thousands)]->SetFrameIndex(thousand);
	m_Numbers[ENUM_CLASS(Digit::Hundreads)]->SetFrameIndex(hundread);
	m_Numbers[ENUM_CLASS(Digit::Tens)]->SetFrameIndex(ten);
	m_Numbers[ENUM_CLASS(Digit::Units)]->SetFrameIndex(unit);

	m_iNumber = number;
}

Object* DamageFont::Clone(InitDESC* arg)
{
	DamageFont* Instance = new DamageFont(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DamageFont::Free()
{
	__super::Free();

	for (auto& number : m_Numbers)
		Safe_Release(number);
	m_Numbers.clear();
}

void DamageFont::SetNumberScale(_float scale)
{
	for (_uint i = 0; i < ENUM_CLASS(Digit::Count); ++i)
	{
		m_Numbers[i]->GetMaterialInstance()->SetFloat("g_FontSize", scale);
	}
}

void DamageFont::DamageFontSpawn::Enter(Object* object)
{
	auto random = EngineCore::GetInstance()->GetRandom();
	auto damage = static_cast<DamageFont*>(object);
	
	m_fElapsedTime = 0.f;

	m_fTargetScale = damage->m_fFontSize;
	m_fStartScale = m_fTargetScale * 2.f;

	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_TargetPosition = m_StartPosition;

	m_TargetPosition.x += random->get<_float>(-8.f, 8.f);
	m_TargetPosition.y += random->get<_float>(-8.f, 8.f);
	m_TargetPosition.z += random->get<_float>(-8.f, 8.f);
}

void DamageFont::DamageFontSpawn::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		auto damage = static_cast<DamageFont*>(object);

		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutQuint(t);

		_float3 currPosition{};
		_float currScale = math::Lerp(m_fStartScale, m_fTargetScale, t);
		damage->SetNumberScale(currScale);
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_TargetPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(currPosition);
	}
}

void DamageFont::DamageFontSpawn::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto damage = static_cast<DamageFont*>(object);
		damage->ChangeState(&damage->m_DamageFontDead);
	}
}

void DamageFont::DamageFontDead::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
}

void DamageFont::DamageFontDead::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);
		t = 1.f - t;

		auto damage = static_cast<DamageFont*>(object);
		_uint number = damage->m_iNumber;

		damage->m_Numbers[ENUM_CLASS(Digit::Thousands)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", t);
		damage->m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", t);
		damage->m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", t);
		damage->m_Numbers[ENUM_CLASS(Digit::Units)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", t);

		if (number < 1000)
			damage->m_Numbers[ENUM_CLASS(Digit::Thousands)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
		if (number < 100)
			damage->m_Numbers[ENUM_CLASS(Digit::Hundreads)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
		if (number < 10)
			damage->m_Numbers[ENUM_CLASS(Digit::Tens)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 0.f);
	}
}

void DamageFont::DamageFontDead::TestForExit(Object* object)
{
	if(m_fElapsedTime>=m_fDuration)
		object->SetDead();
}

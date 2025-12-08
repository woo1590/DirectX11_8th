#include "pch.h"
#include "ShieldHit.h"
#include "MaterialInstance.h"
#include "Random.h"

//component
#include "SpriteComponent.h"

ShieldHit::ShieldHit()
	:Object()
{
}

ShieldHit::ShieldHit(const ShieldHit& prototype)
	:Object(prototype)
{
}

ShieldHit* ShieldHit::Create()
{
	ShieldHit* Instance = new ShieldHit();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT ShieldHit::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT ShieldHit::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	_uint randNum = engine->GetRandom()->get<_uint>(0, 1);

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.iMaxFrameIndex = 1;
	spriteDesc.isAnimated = false;
	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);
	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Point");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_ShieldHit");
//	sprite->GetMaterialInstance()->SetPass("CustomAlpha_Pass");
	sprite->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f);

	m_StartScale = _float3{ 10.f,10.f,1.f };
	m_EndScale = _float3{ 60.f,60.f,1.f };
	m_fStartAlpha = 1.f;
	m_fEndAlpha = 0.f;

	if (randNum == 0)
		m_pTransform->Rotate(_float3{ 0.f,0.f,math::ToRadian(90.f) });

	return S_OK;
}

void ShieldHit::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void ShieldHit::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
	{
		SetDead();
		return;
	}

	_float t = m_fElapsedTime / m_fDuration;
	t = math::EaseOutQuint(t);

	_float3 currScale{};
	_float currAlpha{};
	XMStoreFloat3(&currScale, XMVectorLerp(XMLoadFloat3(&m_StartScale), XMLoadFloat3(&m_EndScale), t));
	currAlpha = math::Lerp(m_fStartAlpha, m_fEndAlpha, t);

	auto mtrlInstance = GetComponent<SpriteComponent>()->GetMaterialInstance();
	mtrlInstance->SetFloat("g_CustomAlpha", currAlpha);

	m_pTransform->SetScale(currScale);
}

void ShieldHit::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* ShieldHit::Clone(InitDESC* arg)
{
	ShieldHit* Instance = new ShieldHit(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void ShieldHit::Free()
{
	__super::Free();
}

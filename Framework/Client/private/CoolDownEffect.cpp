#include "pch.h"
#include "CoolDownEffect.h"

//component
#include "SpriteComponent.h"

CoolDownEffect::CoolDownEffect()
	:UIObject()
{
}

CoolDownEffect::CoolDownEffect(const CoolDownEffect& prototype)
	:UIObject(prototype)
{
}

CoolDownEffect* CoolDownEffect::Create()
{
	CoolDownEffect* Instance = new CoolDownEffect();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CoolDownEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<SpriteComponent>();
	//m_eRenderGroup = RenderGroup::UI;
	
	return S_OK;
}

HRESULT CoolDownEffect::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.fSpeed = 30.f;
	spriteDesc.iMaxFrameIndex = 6;
	spriteDesc.isAnimated = true;
	spriteDesc.isRepeat = false;

	auto sprite = GetComponent<SpriteComponent>();
	sprite->Initialize(&spriteDesc);

	sprite->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	sprite->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_CoolDownEffect");

	m_fSizeX = 238.f*0.8f;
	m_fSizeY = 449.f * 0.8f;
	_float3 position{ WinSizeX * 0.5f - WinSizeX * 0.23f, WinSizeY * -0.5f + WinSizeY * 0.12,0.f };

	m_pTransform->SetScale(_float3{ m_fSizeX,m_fSizeY,1.f });
	m_pTransform->SetPosition(position);

	return S_OK;
}

void CoolDownEffect::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void CoolDownEffect::Update(_float dt)
{
	__super::Update(dt);

	auto sprite = GetComponent<SpriteComponent>();
	if (sprite->IsFinished())
		SetDead();
}

void CoolDownEffect::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT CoolDownEffect::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	return __super::ExtractRenderProxies(proxies);
}

Object* CoolDownEffect::Clone(InitDESC* arg)
{
	CoolDownEffect* Instance = new CoolDownEffect(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void CoolDownEffect::Free()
{
	__super::Free();
}

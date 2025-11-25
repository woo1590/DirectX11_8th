#include "pch.h"
#include "HitCrossHair.h"
#include "MaterialInstance.h"

//component
#include "SpriteComponent.h"

HitCrossHair::HitCrossHair()
	:UIObject()
{
}

HitCrossHair::HitCrossHair(const HitCrossHair& prototype)
	:UIObject(prototype)
{
}

HitCrossHair* HitCrossHair::Create()
{
	HitCrossHair* Instance = new HitCrossHair();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT HitCrossHair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT HitCrossHair::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	engine->Subscribe(ENUM_CLASS(EventID::Hit), MakeListener(&HitCrossHair::Hit));
	engine->Subscribe(ENUM_CLASS(EventID::WeaknessHit), MakeListener(&HitCrossHair::HitWeakness));

	/*sprite*/
	SpriteComponent::SPRITE_DESC spriteDesc{};
	spriteDesc.isAnimated = false;
	spriteDesc.iMaxFrameIndex = 1;

	m_CrossHairs.resize(ENUM_CLASS(Parts::Count));

	m_CrossHairs[ENUM_CLASS(Parts::LT)] = SpriteComponent::Create(this);
	m_CrossHairs[ENUM_CLASS(Parts::LT)]->Initialize(&spriteDesc);
	m_CrossHairs[ENUM_CLASS(Parts::LT)]->SetBuffer(ENUM_CLASS(LevelID::Static),"Buffer_Quad");
	m_CrossHairs[ENUM_CLASS(Parts::LT)]->SetMaterial(ENUM_CLASS(LevelID::Static),"Mtrl_HitCrossHair");
	m_CrossHairs[ENUM_CLASS(Parts::LT)]->GetMaterialInstance()->SetPass("HitCrossHair_Pass");

	m_CrossHairs[ENUM_CLASS(Parts::RT)] = SpriteComponent::Create(this);
	m_CrossHairs[ENUM_CLASS(Parts::RT)]->Initialize(&spriteDesc);
	m_CrossHairs[ENUM_CLASS(Parts::RT)]->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_CrossHairs[ENUM_CLASS(Parts::RT)]->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_HitCrossHair");
	m_CrossHairs[ENUM_CLASS(Parts::RT)]->GetMaterialInstance()->SetPass("HitCrossHair_Pass");

	m_CrossHairs[ENUM_CLASS(Parts::RB)] = SpriteComponent::Create(this);
	m_CrossHairs[ENUM_CLASS(Parts::RB)]->Initialize(&spriteDesc);
	m_CrossHairs[ENUM_CLASS(Parts::RB)]->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_CrossHairs[ENUM_CLASS(Parts::RB)]->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_HitCrossHair");
	m_CrossHairs[ENUM_CLASS(Parts::RB)]->GetMaterialInstance()->SetPass("HitCrossHair_Pass");

	m_CrossHairs[ENUM_CLASS(Parts::LB)] = SpriteComponent::Create(this);
	m_CrossHairs[ENUM_CLASS(Parts::LB)]->Initialize(&spriteDesc);
	m_CrossHairs[ENUM_CLASS(Parts::LB)]->SetBuffer(ENUM_CLASS(LevelID::Static), "Buffer_Quad");
	m_CrossHairs[ENUM_CLASS(Parts::LB)]->SetMaterial(ENUM_CLASS(LevelID::Static), "Mtrl_HitCrossHair");
	m_CrossHairs[ENUM_CLASS(Parts::LB)]->GetMaterialInstance()->SetPass("HitCrossHair_Pass");

	/*transform*/
	m_Transforms.resize(ENUM_CLASS(Parts::Count));

	m_Transforms[ENUM_CLASS(Parts::LT)] = TransformComponent::Create(this);
	m_Transforms[ENUM_CLASS(Parts::LT)]->Rotate(_float3{ 0.f,0.f,math::ToRadian(45.f) });

	m_Transforms[ENUM_CLASS(Parts::RT)] = TransformComponent::Create(this);
	m_Transforms[ENUM_CLASS(Parts::RT)]->Rotate(_float3{ 0.f,0.f,math::ToRadian(-45.f) });

	m_Transforms[ENUM_CLASS(Parts::RB)] = TransformComponent::Create(this);
	m_Transforms[ENUM_CLASS(Parts::RB)]->Rotate(_float3{ 0.f,0.f,math::ToRadian(-135.f) });

	m_Transforms[ENUM_CLASS(Parts::LB)] = TransformComponent::Create(this);
	m_Transforms[ENUM_CLASS(Parts::LB)]->Rotate(_float3{ 0.f,0.f,math::ToRadian(-225.f) });


	m_StartPosition = _float3{ -20.f,20.f,0.f };
	m_EndPosition = _float3{ -10.f,10.f,0. };

	return S_OK;
}

void HitCrossHair::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void HitCrossHair::Update(_float dt)
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
		_float progress = math::EaseInQuint(t);

		if (m_IsWeakness)
			t = math::EaseOutQuint(t);
		else
			t = math::PalabolaCurve(t);

		_float3 currPosition{};
		_float3 startPosition = m_StartPosition;
		_float3 endPosition = m_EndPosition;

		/*LT*/
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&endPosition), t));
		m_Transforms[ENUM_CLASS(Parts::LT)]->SetPosition(currPosition);
		m_CrossHairs[ENUM_CLASS(Parts::LT)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f - progress);

		/*RT*/
		startPosition.x = -1.f * m_StartPosition.x;
		endPosition.x = -1.f * m_EndPosition.x;
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&endPosition), t));
		m_Transforms[ENUM_CLASS(Parts::RT)]->SetPosition(currPosition);
		m_CrossHairs[ENUM_CLASS(Parts::RT)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f - progress);

		/*RB*/
		startPosition.y = -m_StartPosition.y;
		endPosition.y = -m_EndPosition.y;
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&endPosition), t));
		m_Transforms[ENUM_CLASS(Parts::RB)]->SetPosition(currPosition);
		m_CrossHairs[ENUM_CLASS(Parts::RB)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f - progress);

		/*LB*/
		startPosition = m_StartPosition;
		endPosition = m_EndPosition;
		startPosition.y *= -1.f;
		endPosition.y *= -1.f;
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&endPosition), t));
		m_Transforms[ENUM_CLASS(Parts::LB)]->SetPosition(currPosition);
		m_CrossHairs[ENUM_CLASS(Parts::LB)]->GetMaterialInstance()->SetFloat("g_CustomAlpha", 1.f - progress);
	}
}

void HitCrossHair::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT HitCrossHair::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	if (!m_IsActive)
		return S_OK;

	for (_uint i = 0; i < ENUM_CLASS(Parts::Count); ++i)
	{
		m_CrossHairs[i]->ExtractRenderProxy(m_Transforms[i], proxies[ENUM_CLASS(RenderGroup::UI)]);
	}
	//m_CrossHairs[0]->ExtractRenderProxy(m_Transforms[0], proxies[ENUM_CLASS(RenderGroup::UI)]);
	return S_OK;
}

void HitCrossHair::Hit(std::any param)
{
	m_IsActive = true;
	m_IsWeakness = false;
	m_fElapsedTime = 0.f;

	for(const auto& transform : m_Transforms)
		transform->SetScale(_float3{ 3.f,20.f,1.f });

	for (const auto& sprite : m_CrossHairs)
		sprite->GetMaterialInstance()->SetFloat4("g_Color", _float4(1.f, 1.f, 1.f, 1.f));

	m_StartPosition = _float3{ -20.f,20.f,0.f };
	m_EndPosition = _float3{ -10.f,10.f,0. };
}

void HitCrossHair::HitWeakness(std::any param)
{
	m_IsActive = true;
	m_IsWeakness = true;
	m_fElapsedTime = 0.f;

	for (const auto& transform : m_Transforms)
		transform->SetScale(_float3{ 9.f,20.f * 2.f,1.f });

	for (const auto& sprite : m_CrossHairs)
		sprite->GetMaterialInstance()->SetFloat4("g_Color", _float4(1.f, 0.f, 0.f, 1.f));

	m_StartPosition = _float3{ -20.f,20.f,0.f };
	m_EndPosition = _float3{ -30.f,30.f,0. };
}

Object* HitCrossHair::Clone(InitDESC* arg)
{
	HitCrossHair* Instance = new HitCrossHair(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void HitCrossHair::Free()
{
	__super::Free();

	for (auto& crossHair : m_CrossHairs)
		Safe_Release(crossHair);
	m_CrossHairs.clear();

	for (auto& transform : m_Transforms)
		Safe_Release(transform);
	m_Transforms.clear();
}

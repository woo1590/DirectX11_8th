#include "pch.h"
#include "DefaultBulletTrail.h"

//component
#include "TrailComponent.h"

DefaultBulletTrail::DefaultBulletTrail()
	:Object()
{
}

DefaultBulletTrail::DefaultBulletTrail(const DefaultBulletTrail& prototype)
	:Object(prototype)
{
}

DefaultBulletTrail* DefaultBulletTrail::Create()
{
	DefaultBulletTrail* Instance = new DefaultBulletTrail();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DefaultBulletTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<TrailComponent>();
	m_eRenderGroup = RenderGroup::Blend;

	return S_OK;
}

HRESULT DefaultBulletTrail::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	DEFAULT_BULLET_TRAIL_DESC* desc = static_cast<DEFAULT_BULLET_TRAIL_DESC*>(arg);
	m_CurrPoint = desc->startPosition;

	auto engine = EngineCore::GetInstance();

	/*trail*/
	TrailComponent::TRAIL_DESC trailDesec{};
	trailDesec.width = 2.f;
	auto trail = GetComponent<TrailComponent>();
	trail->Initialize(&trailDesec);
	trail->SetMaterial(ENUM_CLASS(LevelID::Static), desc->mtrlTag);

	return S_OK;
}

void DefaultBulletTrail::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DefaultBulletTrail::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
		m_IsActive = true;
}

void DefaultBulletTrail::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void DefaultBulletTrail::AddNextPosition(_float3 position)
{

	if (m_IsActive)
	{
		m_PrevPoint = m_CurrPoint;
		m_CurrPoint = position;

		_float3 p0{}, p1{};
		_vector dir = XMVector3Normalize(XMLoadFloat3(&m_CurrPoint) - XMLoadFloat3(&m_PrevPoint));
	
		p1 = m_CurrPoint;
		XMStoreFloat3(&p0, XMLoadFloat3(&m_CurrPoint) - dir * 40.f);

		GetComponent<TrailComponent>()->AddPoints(p0, p1);
	}
}

Object* DefaultBulletTrail::Clone(InitDESC* arg)
{
	DefaultBulletTrail* Instance = new DefaultBulletTrail(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DefaultBulletTrail::Free()
{
	__super::Free();
}

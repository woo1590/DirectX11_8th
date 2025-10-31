#include "pch.h"
#include "BossPillar.h"
#include "Random.h"
#include "Bounding_AABB.h"

//component
#include "ColliderComponent.h"
#include "ModelComponent.h"

BossPillar::BossPillar()
	:Object()
{
}

BossPillar::BossPillar(const BossPillar& prototype)
	:Object(prototype)
{
}

BossPillar* BossPillar::Create()
{
	BossPillar* Instance = new BossPillar();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossPillar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<ModelComponent>();

	m_strInstanceTag = "Boss_Pillar";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT BossPillar::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BossPillar);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,22.f,0.f };
	aabbDesc.halfSize = _float3{ 9.f,25.f,9.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);
	engine->RegisterCollider(collider);

	/*model*/	
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_BossPillar");

	ChangeState(&m_BossPillarSpawn);

	return S_OK;
}

void BossPillar::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossPillar::Update(_float dt)
{
	__super::Update(dt);
}

void BossPillar::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void BossPillar::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::BossStoneProjectile:
	{
		--m_iResistantCount;
		if (!m_iResistantCount)
			SetDead();
	}break;
	default:
		break;
	}
}

void BossPillar::Explode()
{
	if (m_CurrState == &m_BossPillarIdle)
		ChangeState(&m_BossPillarExplode);
}

Object* BossPillar::Clone(InitDESC* arg)
{
	BossPillar* Instance = new BossPillar(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossPillar::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void BossPillar::BossPillarSpawn::Enter(Object* object)
{
	auto engine = EngineCore::GetInstance();
	auto pillar = static_cast<BossPillar*>(object);

	m_fElapsedTime = 0.f;
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_EndPosition = m_StartPosition;
	m_EndPosition.y = -13.f;
}

void BossPillar::BossPillarSpawn::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	if (m_fElapsedTime < m_fDuration)
	{
		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);

		_float3 currPosition{};
		XMStoreFloat3(&currPosition, XMVectorLerp(XMLoadFloat3(&m_StartPosition), XMLoadFloat3(&m_EndPosition), t));

		object->GetComponent<TransformComponent>()->SetPosition(currPosition);
	}
}

void BossPillar::BossPillarSpawn::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto pillar = static_cast<BossPillar*>(object);

		pillar->ChangeState(&pillar->m_BossPillarIdle);
	}
}

void BossPillar::BossPillarExlplode::Enter(Object* object)
{
	auto random = EngineCore::GetInstance()->GetRandom();

	m_fElapsedTime = 0.f;
	m_fDuration = random->get<_float>(2.f, 3.5f);
	m_StartPosition = object->GetComponent<TransformComponent>()->GetPosition();
}

void BossPillar::BossPillarExlplode::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt; 

	if (m_fElapsedTime < m_fDuration)
	{
		auto random = EngineCore::GetInstance()->GetRandom();
		auto transform = object->GetComponent<TransformComponent>();

		_float t = m_fElapsedTime / m_fDuration;
		t = std::clamp(t, 0.f, 1.f);
		_float3 offset{};
		_float3 position = transform->GetPosition();
		offset.x = random->get<_float>(-m_fShakePower, m_fShakePower) * t * t;
		offset.y = random->get<_float>(-m_fShakePower * 0.5f, m_fShakePower * 0.5f) * t * t;
		offset.z = random->get<_float>(-m_fShakePower, m_fShakePower) * t * t;

		XMStoreFloat3(&position, XMLoadFloat3(&m_StartPosition) + XMLoadFloat3(&offset));
		transform->SetPosition(position);
	}
}

void BossPillar::BossPillarExlplode::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
		object->SetDead();
}

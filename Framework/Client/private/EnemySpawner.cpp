#include "pch.h"
#include "EnemySpawner.h"
#include "Random.h"
#include "Bounding_AABB.h"

//component
#include "NavigationComponent.h"
#include "ColliderComponent.h"

EnemySpawner::EnemySpawner()
	:Object()
{
}

EnemySpawner::EnemySpawner(const EnemySpawner& prototype)
	:Object(prototype)
{
}

EnemySpawner* EnemySpawner::Create()
{
	EnemySpawner* Instance = new EnemySpawner();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT EnemySpawner::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT EnemySpawner::Initialize(InitDESC* arg)
{
	ENEMY_SPAWNER_DESC* desc = static_cast<ENEMY_SPAWNER_DESC*>(arg);
	m_AvailableNavCellIndices = std::move(desc->availableNavCellIndices);
	m_Waves = std::move(desc->waves);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.halfSize = _float3{ 10.f,10.f,10.f };

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);

	m_pTransform->SetPosition(_float3{ -316.f,5.f,42.f });

	return S_OK;
}

void EnemySpawner::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void EnemySpawner::Update(_float dt)
{
	__super::Update(dt);
}

void EnemySpawner::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	auto player = EngineCore::GetInstance()->GetFrontObject(ENUM_CLASS(LevelID::GamePlay), "Layer_Player");
	auto collider = GetComponent<ColliderComponent>();

	if (collider->Intersect(player->GetComponent<ColliderComponent>()))
		Spawn();
}

void EnemySpawner::OnCollisionEnter(ColliderComponent* collider, ColliderComponent* otherCollider)
{
	Spawn();
}

Object* EnemySpawner::Clone(InitDESC* arg)
{
	EnemySpawner* Instance = new EnemySpawner(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void EnemySpawner::Free()
{
	__super::Free();
}

void EnemySpawner::Spawn()
{
	if (m_IsSpawned)
		return;

	auto engine = EngineCore::GetInstance();
	for (const auto& entry : m_Waves[0])
	{
		for (_uint i = 0; i < entry.count; ++i)
		{
			_uint rand = engine->GetRandom()->get<_uint>(0, m_AvailableNavCellIndices.size() - 1);
			Object* enemy = nullptr;
			engine->AddObject(ENUM_CLASS(LevelID::GamePlay), entry.prototypeTag, ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy", nullptr, &enemy);

			enemy->GetComponent<NavigationComponent>()->SpawnInCell(m_AvailableNavCellIndices[rand]);
		}
	}

	m_IsSpawned = true;
}

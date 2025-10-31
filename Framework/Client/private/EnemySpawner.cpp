#include "pch.h"
#include "EnemySpawner.h"
#include "Random.h"
#include "Bounding_Sphere.h"

//object
#include "Door.h"

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

	if (desc->doorID != -1)
	{
		auto engine = EngineCore::GetInstance();
		auto& doors = engine->GetObjects(ENUM_CLASS(LevelID::GamePlay), "Layer_Door");

		auto iter = doors.begin();
		for (_uint i = 0; i < desc->doorID; ++i)
			iter++;

		m_pConnectedDoor = dynamic_cast<Door*>((*iter));
		if (!m_pConnectedDoor)
			return E_FAIL;
	}

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Spawner);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 10.f;

	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	EngineCore::GetInstance()->RegisterCollider(collider);

	ChangeState(&m_SpawnerIdle);

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
}

void EnemySpawner::OnCollisionEnter(ColliderComponent* otherCollider)
{
	switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
	{
	case ColliderFilter::Player:
	{
		if (m_CurrState == &m_SpawnerIdle)
		{
			if (m_Waves.empty())
				ChangeState(&m_SpawnerEnd);
			else
				ChangeState(&m_SpawnerSpawn);
		}
	}break;
	default:
		break;
	}
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
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void EnemySpawner::SpawnerSpawn::Enter(Object* object)
{
	auto spawner = static_cast<EnemySpawner*>(object);

	auto engine = EngineCore::GetInstance();
	for (const auto& entry : spawner->m_Waves[spawner->m_iCurrWave])
	{
		for (_uint i = 0; i < entry.count; ++i)
		{
			_uint rand = engine->GetRandom()->get<_uint>(0, spawner->m_AvailableNavCellIndices.size() - 1);
			Object* enemy = nullptr;
			engine->AddObject(ENUM_CLASS(LevelID::GamePlay), entry.prototypeTag, ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy", nullptr, &enemy);

			enemy->GetComponent<NavigationComponent>()->SpawnInCell(spawner->m_AvailableNavCellIndices[rand]);
			spawner->m_CurrWaveEnemies.push_back(enemy);
		}
	}
}

void EnemySpawner::SpawnerSpawn::TestForExit(Object* object)
{
	auto spawner = static_cast<EnemySpawner*>(object);
	spawner->ChangeState(&spawner->m_SpawnerWaveRunning);
}

void EnemySpawner::SpawnerWaveRunning::Update(Object* object, _float dt)
{
	auto spawner = static_cast<EnemySpawner*>(object);
	for (auto iter = spawner->m_CurrWaveEnemies.begin(); iter != spawner->m_CurrWaveEnemies.end();)
	{
		if ((*iter)->IsDead())
			iter = spawner->m_CurrWaveEnemies.erase(iter);
		else
			++iter;
	}

}

void EnemySpawner::SpawnerWaveRunning::TestForExit(Object* object)
{
	auto spawner = static_cast<EnemySpawner*>(object);

	if (spawner->m_CurrWaveEnemies.empty())
	{
		++spawner->m_iCurrWave;
		if (spawner->m_iCurrWave >= spawner->m_Waves.size())
			spawner->ChangeState(&spawner->m_SpawnerEnd);
		else
			spawner->ChangeState(&spawner->m_SpawnerSpawn);
	}
}

void EnemySpawner::SpawnerEnd::Enter(Object* object)
{
	auto spawner = static_cast<EnemySpawner*>(object);

	if (spawner->m_pConnectedDoor)
		spawner->m_pConnectedDoor->Open();
}

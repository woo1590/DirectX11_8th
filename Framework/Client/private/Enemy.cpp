#include "pch.h"
#include "Enemy.h"
#include "Random.h"
#include "EnemyHpPanel.h"

//component
#include "RigidBodyComponent.h"

_uint Enemy::m_iNextEnemyID = 0;

Enemy::Enemy()
	:ContainerObject()
{
}

Enemy::Enemy(const Enemy& prototype)
	:ContainerObject(prototype)
{
}

HRESULT Enemy::Initialize(InitDESC* arg)
{
    if (FAILED(__super::Initialize(arg)))
        return E_FAIL;

    m_UseShadow = true;

    return S_OK;
}

HRESULT Enemy::LateInitialize()
{
    __super::LateInitialize();

    auto engine = EngineCore::GetInstance();

    m_iEnemyID = m_iNextEnemyID++;

    EnemyHpPanel::ENEMY_HP_PANEL_DESC desc{};
    desc.ownerID = m_iEnemyID;
    desc.scale = _float3{ 1.f,1.f,1.f };
    engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyHpPanel", engine->GetCurrLevelID(), "Layer_UI", &desc);

    return S_OK;
}

void Enemy::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
}

void Enemy::Free()
{
	__super::Free();
}

void Enemy::SetDead()
{
    __super::SetDead();

	auto engine = EngineCore::GetInstance();
	auto random = engine->GetRandom();
    

    EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
    param.ownerID = m_iEnemyID;
    engine->PublishEvent(ENUM_CLASS(EventID::EnemyDead), param);

	_uint numCoins = random->get<_uint>(4, 6);
    for (_uint i = 0; i < 5; ++i)
    {
        _float3 position = m_pTransform->GetPosition();
        _float3 spawnPosition{};
        spawnPosition.x = position.x + random->get<_float>(-4.f, 4.f);
        spawnPosition.y = position.y + random->get<_float>(5.f, 8.f);
        spawnPosition.z = position.z + random->get<_float>(-4.f, 4.f);

        _float3 velocity{};
        _float power = random->get<_float>(50.f, 70.f);
        _vector dir = XMVector3Normalize(XMLoadFloat3(&spawnPosition) - XMLoadFloat3(&position));
        XMStoreFloat3(&velocity, dir * power);

        Object* coin = nullptr;
        Object::OBJECT_DESC coinDesc{};
        coinDesc.position = spawnPosition;
        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Coin", engine->GetCurrLevelID(), "Layer_Item", &coinDesc, &coin);

        auto itemRigidBody = coin->GetComponent<RigidBodyComponent>();
        itemRigidBody->SetVelocity(velocity);
    }

    _uint randNum = random->get<_uint>(0, 2);
    if (randNum < 1)
    {
        _float3 position = m_pTransform->GetPosition();
        _float3 spawnPosition{};
        spawnPosition.x = position.x + random->get<_float>(-4.f, 4.f);
        spawnPosition.y = position.y + random->get<_float>(5.f, 8.f);
        spawnPosition.z = position.z + random->get<_float>(-4.f, 4.f);

        _float3 velocity{};
        _float power = random->get<_float>(50.f, 70.f);
        _vector dir = XMVector3Normalize(XMLoadFloat3(&spawnPosition) - XMLoadFloat3(&position));
        XMStoreFloat3(&velocity, dir * power);

        Object* dumpling = nullptr;
        Object::OBJECT_DESC dumplingDesc{};
        dumplingDesc.position = spawnPosition;
        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Dumpling", engine->GetCurrLevelID(), "Layer_Item", &dumplingDesc, &dumpling);

        auto itemRigidBody = dumpling->GetComponent<RigidBodyComponent>();
        itemRigidBody->SetVelocity(velocity);
    }

    randNum = random->get<_uint>(0, 1);
    if (1 == randNum)
    {
        _float3 position = m_pTransform->GetPosition();
        _float3 spawnPosition{};
        spawnPosition.x = position.x + random->get<_float>(-4.f, 4.f);
        spawnPosition.y = position.y + random->get<_float>(5.f, 8.f);
        spawnPosition.z = position.z + random->get<_float>(-4.f, 4.f);

        _float3 velocity{};
        _float power = random->get<_float>(50.f, 70.f);
        _vector dir = XMVector3Normalize(XMLoadFloat3(&spawnPosition) - XMLoadFloat3(&position));
        XMStoreFloat3(&velocity, dir * power);

        Object* ammo = nullptr;
        Object::OBJECT_DESC ammoDesc{};
        ammoDesc.position = spawnPosition;
        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Ammo", engine->GetCurrLevelID(), "Layer_Item", &ammoDesc, &ammo);

        auto itemRigidBody = ammo->GetComponent<RigidBodyComponent>();
        itemRigidBody->SetVelocity(velocity);
    }
}

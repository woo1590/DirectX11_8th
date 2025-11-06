#include "pch.h"
#include "Enemy.h"
#include "Random.h"

//component
#include "RigidBodyComponent.h"

Enemy::Enemy()
	:ContainerObject()
{
}

Enemy::Enemy(const Enemy& prototype)
	:ContainerObject(prototype)
{
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

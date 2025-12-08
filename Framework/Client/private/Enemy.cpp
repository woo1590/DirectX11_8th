#include "pch.h"
#include "Enemy.h"
#include "Random.h"
#include "EnemyHpPanel.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

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

void Enemy::PriorityUpdate(_float dt)
{
    __super::PriorityUpdate(dt);

    if (m_pOutLineModel)
    {
        if (m_IsLockOn)
        {
            m_IsLockOn = false;
            GetComponent<ModelComponent>()->GetMaterialInstance()->SetPass("Default_Pass");

            auto mtrlInstance = m_pOutLineModel->GetMaterialInstance();
            mtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
            mtrlInstance->SetFloat("g_OutLineWidth", 0.1f);
            mtrlInstance->SetPass("OutLine_Pass");
            mtrlInstance->SetInt("g_ObjectMask", 1);
        }
    }
}

void Enemy::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
}

void Enemy::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);

    if (m_pOutLineModel)
    {
        if (m_IsLockOn)
        {
            GetComponent<ModelComponent>()->GetMaterialInstance()->SetPass("LockOn_Pass");

            auto mtrlInstance = m_pOutLineModel->GetMaterialInstance();
            mtrlInstance->SetFloat4("g_OutLineColor", _float4(1.f, 0.2f, 0.2f, 1.f));
            mtrlInstance->SetFloat("g_OutLineWidth", 0.2f);
            mtrlInstance->SetPass("LockOnOutLine_Pass");
        }
    }
}

HRESULT Enemy::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
    __super::ExtractRenderProxies(proxies);

    if (m_pOutLineModel)
        m_pOutLineModel->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonLight)]);

    return S_OK;
}

void Enemy::OnCollisionEnter(ColliderComponent* otherCollider)
{
    if (otherCollider->GetFilter() == ENUM_CLASS(ColliderFilter::PlayerProjectile))
        EngineCore::GetInstance()->Play2DSound("SFX_Hit", 0.6f);
}

void Enemy::Free()
{
	__super::Free();
    Safe_Release(m_pOutLineModel);
}

void Enemy::SetDead()
{
    __super::SetDead();

	auto engine = EngineCore::GetInstance();
	auto random = engine->GetRandom();
    if (!m_IsExplodeDead)
    {
        engine->Play2DSound("SFX_EnemyDead", 0.6f);
        engine->Play2DSound("SFX_EnemySpread", 0.6f);
    }
    else
        engine->Play2DSound("SFX_EnemyDead", 0.3f);
    
    /*----dead effect----*/
    _float3 playerPos = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player")->GetComponent<TransformComponent>()->GetPosition();
    EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
    effectDesc.position = m_pTransform->GetPosition();
    effectDesc.position.y += 5.f;
    XMStoreFloat3(&effectDesc.forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&effectDesc.position)));
    engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_EnemyDeadSmoke", engine->GetCurrLevelID(), "Layer_Effect", &effectDesc);

    EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
    param.ownerID = m_iEnemyID;
    engine->PublishEvent(ENUM_CLASS(EventID::EnemyDead), param);

	_uint numCoins = random->get<_uint>(1, 3);
    for (_uint i = 0; i < numCoins; ++i)
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

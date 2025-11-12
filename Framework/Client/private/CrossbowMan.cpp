#include "pch.h"
#include "CrossbowMan.h"
#include "Bounding_AABB.h"
#include "Random.h"
#include "DamageFont.h"

#include "Fracture.h"
#include "EnemyHpPanel.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "NavigationComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"

CrossbowMan::CrossbowMan()
    :Enemy()
{
}

CrossbowMan::CrossbowMan(const CrossbowMan& prototype)
    :Enemy(prototype)
{
}

CrossbowMan* CrossbowMan::Create()
{
    CrossbowMan* Instance = new CrossbowMan();

    if (FAILED(Instance->Initialize_Prototype()))
        Safe_Release(Instance);

    return Instance;
}

HRESULT CrossbowMan::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    AddComponent<ColliderComponent>();
    AddComponent<StatusComponent>();
    AddComponent<NavigationComponent>();
    AddComponent<RigidBodyComponent>();
    AddComponent<ModelComponent>();
    AddComponent<AnimatorComponent>();

    return S_OK;
}

HRESULT CrossbowMan::Initialize(InitDESC* arg)
{
    ContainerObject::CONTAINER_OBJECT_DESC desc{};
    desc.numPartObjects = ENUM_CLASS(Parts::Count);

    if (FAILED(__super::Initialize(&desc)))
        return E_FAIL;

    auto engine = EngineCore::GetInstance();

    /*collider*/
    Bounding_AABB::AABB_DESC aabbDesc{};
    aabbDesc.useResolve = true;
    aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
    aabbDesc.type = ColliderType::AABB;
    aabbDesc.center = _float3{ 0.f,3.5f,0.f };
    aabbDesc.halfSize = _float3{ 3.f,3.5f,3.f };
    auto collider = GetComponent<ColliderComponent>();
    collider->Initialize(&aabbDesc);
    engine->RegisterCollider(collider);

    /*status*/
    StatusComponent::STATUS_DESC statusDesc{};
    statusDesc.hp = 100;
    auto status = GetComponent<StatusComponent>();
    status->Initialize(&statusDesc);

    /*model*/
    auto model = GetComponent<ModelComponent>();
    model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_CrossbowMan");

    /*animator*/
    auto animator = GetComponent<AnimatorComponent>();
    animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Enemy_CrossbowMan");

    model->ConnectAnimator();

    /*navigation*/
    auto nav = GetComponent<NavigationComponent>();
    engine->RegisterNavigation(nav);
    nav->AttachTransform();
    nav->AttachRigidBody();
    nav->SpawnInCell(7);
    nav->SetMoveSpeed(35.f);
    nav->SetArriveRange(60.f);

    m_iHpPanelBoneIndex = model->GetBoneIndex("MonsterHp");
    m_iMuzzleBoneIndex = model->GetBoneIndex("muzzle");
    m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });
    ChangeState(&m_CrossbowManShow);

    return S_OK;
}

void CrossbowMan::PriorityUpdate(_float dt)
{
    __super::PriorityUpdate(dt);
}

void CrossbowMan::Update(_float dt)
{
    __super::Update(dt);

    _float4x4 hpPanelMat = GetComponent<ModelComponent>()->GetCombinedMatrixByIndex(m_iHpPanelBoneIndex);
    _float4x4 worldMat = m_pTransform->GetWorldMatrix();
    _matrix panelMat = XMLoadFloat4x4(&hpPanelMat) * XMLoadFloat4x4(&worldMat);
    _vector positionV, scaleV, rotV;
    _float3 position{};
    XMMatrixDecompose(&scaleV, &rotV, &positionV, panelMat);
    XMStoreFloat3(&position, positionV);

    auto engine = EngineCore::GetInstance();
    EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
    param.ownerID = m_iEnemyID;
    param.position = position;

    engine->PublishEvent(ENUM_CLASS(EventID::EnemyHpPanelPositionUpdate), param);
}

void CrossbowMan::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);
}

void CrossbowMan::HitHead()
{
    auto status = GetComponent<StatusComponent>();

    if (0 == status->GetDesc().hp)
        ChangeState(&m_CrossbowManDead);

    if (m_CurrState == &m_CrossbowManIdle || m_CurrState == &m_CrossbowManRun)
    {
        if (m_fElapsedTime >= m_fHitDelay)
        {
            ChangeState(&m_CrossbowManHitHead);
            m_fElapsedTime = 0.f;
        }
    }
    auto engine = EngineCore::GetInstance();
    auto random = engine->GetRandom();

    DamageFont::DAMAGE_FONT_DESC desc{};
    desc.position = m_pTransform->GetPosition();
    desc.position.y += 5.f;
    desc.fontSize = 0.06f;
    desc.number = random->get<_uint>(900, 1200);
    desc.color = _float4{ 1.f,1.f,0.f,1.f };

    engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", engine->GetCurrLevelID(), "Layer_UI", &desc);

    EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
    param.ownerID = m_iEnemyID;
    param.ratio = status->GetHpRatio();
    engine->PublishEvent(ENUM_CLASS(EventID::EnemyHealthDecrease), param);
}

void CrossbowMan::OnCollisionEnter(ColliderComponent* otherCollider)
{
    auto engine = EngineCore::GetInstance();
    auto random = engine->GetRandom();

    switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
    {
    case ColliderFilter::PlayerAttack:
    {
        auto status = GetComponent<StatusComponent>();
        auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

        status->BeAttacked(otherStatus->GetDesc().attackPower);
        if (0 == status->GetDesc().hp)
            ChangeState(&m_CrossbowManDead);

        if (m_CurrState == &m_CrossbowManIdle || m_CurrState == &m_CrossbowManRun || m_CurrState == &m_CrossbowManWalk_F)
        {
            if (m_fElapsedTime >= m_fHitDelay)
            {
                ChangeState(&m_CrossbowManHitBody);
                m_fElapsedTime = 0.f;
            }
        }

        DamageFont::DAMAGE_FONT_DESC desc{};
        desc.position = m_pTransform->GetPosition();
        desc.position.y += 5.f;
        desc.fontSize = 0.04f;
        desc.number = random->get<_uint>(600, 900);
        desc.color = _float4{ 1.f,1.f,1.f,1.f };

        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", engine->GetCurrLevelID(), "Layer_UI", &desc);

        EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
        param.ownerID = m_iEnemyID;
        param.ratio = status->GetHpRatio();
        engine->PublishEvent(ENUM_CLASS(EventID::EnemyHealthDecrease), param);

    }break;
    case ColliderFilter::PlayerProjectile:
    {
        auto status = GetComponent<StatusComponent>();
        auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

        status->BeAttacked(otherStatus->GetDesc().attackPower);
        if (0 == status->GetDesc().hp)
            ChangeState(&m_CrossbowManDead);

        if (m_CurrState == &m_CrossbowManIdle || m_CurrState == &m_CrossbowManRun || m_CurrState == &m_CrossbowManWalk_F)
        {
            if (m_fElapsedTime >= m_fHitDelay)
            {
                ChangeState(&m_CrossbowManHitBody);
                m_fElapsedTime = 0.f;
            }
        }

        DamageFont::DAMAGE_FONT_DESC desc{};
        desc.position = m_pTransform->GetPosition();
        desc.position.y += 5.f;
        desc.fontSize = 0.04f;
        desc.number = random->get<_uint>(600, 900);
        desc.color = _float4{ 1.f,1.f,1.f,1.f };

        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_DamageFont", engine->GetCurrLevelID(), "Layer_UI", &desc);

        EnemyHpPanel::ENEMY_HP_PANEL_PARAM param{};
        param.ownerID = m_iEnemyID;
        param.ratio = status->GetHpRatio();
        engine->PublishEvent(ENUM_CLASS(EventID::EnemyHealthDecrease), param);

    }break;
    default:
        break;
    }
}

Object* CrossbowMan::Clone(InitDESC* arg)
{
    CrossbowMan* Instance = new CrossbowMan(*this);

    if (FAILED(Instance->Initialize(arg)))
        Safe_Release(Instance);

    return Instance;
}

void CrossbowMan::Free()
{
    EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

    __super::Free();
}

HRESULT CrossbowMan::CreatePartObjects()
{
    return S_OK;
}

void CrossbowMan::CrossbowManShow::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Land));
}

void CrossbowMan::CrossbowManShow::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManShow::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManIdle::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat2), true);

    m_fElapsedTime = 0.f;
}

void CrossbowMan::CrossbowManIdle::Update(Object* object, _float dt)
{
    m_fElapsedTime += dt;

    auto engine = EngineCore::GetInstance();
    auto transform = object->GetComponent<TransformComponent>();
    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");

    _float3 position = transform->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();
    position.y = 0.f;
    playerPos.y = 0.f;

    _float3 currDir = transform->GetForward();
    _float3 targetDir{};
    XMStoreFloat3(&targetDir, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));
    XMStoreFloat3(&targetDir, XMVectorLerp(XMLoadFloat3(&currDir), XMLoadFloat3(&targetDir), dt * 5.f));
    transform->SetForward(targetDir);
}

void CrossbowMan::CrossbowManIdle::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    if (m_fElapsedTime >= m_fDuration)
    {
        if (distance >= 100.f)
        {
            auto crossbow = static_cast<CrossbowMan*>(object);
            crossbow->ChangeState(&crossbow->m_CrossbowManRun);
        }
        else
        {
            _uint randNum = engine->GetRandom()->get<_uint>(0, 5);
            auto crossbow = static_cast<CrossbowMan*>(object);

            if (randNum < 3)
                crossbow->ChangeState(&crossbow->m_CrossbowManFire);
            else if (randNum < 4)
                crossbow->ChangeState(&crossbow->m_CrossbowManHide_L);
            else
                crossbow->ChangeState(&crossbow->m_CrossbowManHide_R);
        }
    }
}

void CrossbowMan::CrossbowManRun::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run), true);

    auto nav = object->GetComponent<NavigationComponent>();
    nav->SetMoveSpeed(35.f);
}

void CrossbowMan::CrossbowManRun::Update(Object* object, _float dt)
{
    auto engine = EngineCore::GetInstance();

    m_fElapsedTime += dt;

    if (m_fElapsedTime >= m_fDuration)
    {
        auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
        auto transform = object->GetComponent<TransformComponent>();
        auto nav = object->GetComponent<NavigationComponent>();

        _uint currCellIndex = nav->GetCurrCellIndex();
        _uint targetCellIndex = player->GetComponent<NavigationComponent>()->GetCurrCellIndex();
        _float3 position = transform->GetPosition();
        _float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();

        nav->FindPath(position, currCellIndex, targetPosition, targetCellIndex);
    }

    auto nav = object->GetComponent<NavigationComponent>();
    nav->MoveByPath(dt);
}

void CrossbowMan::CrossbowManRun::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    if (distance < 70.f)
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManWalk_F);
    }
}

void CrossbowMan::CrossbowManWalk_F::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Walk_F), true);

    auto nav = object->GetComponent<NavigationComponent>();
    nav->SetMoveSpeed(15.f);
}

void CrossbowMan::CrossbowManWalk_F::Update(Object* object, _float dt)
{
    auto engine = EngineCore::GetInstance();

    m_fElapsedTime += dt;

    if (m_fElapsedTime >= m_fDuration)
    {
        auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
        auto transform = object->GetComponent<TransformComponent>();
        auto nav = object->GetComponent<NavigationComponent>();

        _uint currCellIndex = nav->GetCurrCellIndex();
        _uint targetCellIndex = player->GetComponent<NavigationComponent>()->GetCurrCellIndex();
        _float3 position = transform->GetPosition();
        _float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();

        nav->FindPath(position, currCellIndex, targetPosition, targetCellIndex);
    }

    auto nav = object->GetComponent<NavigationComponent>();
    nav->MoveByPath(dt);
}

void CrossbowMan::CrossbowManWalk_F::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    if (distance < 50.f)
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManWalk_R::Enter(Object* object)
{
}

void CrossbowMan::CrossbowManWalk_R::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManWalk_R::TestForExit(Object* object)
{
}

void CrossbowMan::CrossbowManWalk_L::Enter(Object* object)
{
}

void CrossbowMan::CrossbowManWalk_L::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManWalk_L::TestForExit(Object* object)
{
}

void CrossbowMan::CrossbowManHide_R::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::HideRight));

    _float3 right = object->GetComponent<TransformComponent>()->GetRight();
    _float3 velocity{};
    XMStoreFloat3(&velocity, XMLoadFloat3(&right) * m_fHideSpeed);
    auto rigidBody = object->GetComponent<RigidBodyComponent>();

    rigidBody->SetVelocity(velocity);
}

void CrossbowMan::CrossbowManHide_R::Update(Object* object, _float dt)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    _float progress = animator->GetProgress();
    if (progress < 0.5f)
    {
        auto nav = object->GetComponent<NavigationComponent>();
        nav->MoveByVelocity(dt);
    }
}

void CrossbowMan::CrossbowManHide_R::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManHide_L::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::HideLeft));

    _float3 right = object->GetComponent<TransformComponent>()->GetRight();
    _float3 velocity{};
    XMStoreFloat3(&velocity, XMLoadFloat3(&right) * m_fHideSpeed * -1.f);
    auto rigidBody = object->GetComponent<RigidBodyComponent>();

    rigidBody->SetVelocity(velocity);
}

void CrossbowMan::CrossbowManHide_L::Update(Object* object, _float dt)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    _float progress = animator->GetProgress();
    if (progress < 0.5f)
    {
        auto nav = object->GetComponent<NavigationComponent>();
        nav->MoveByVelocity(dt);
    }
}

void CrossbowMan::CrossbowManHide_L::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManFire::Enter(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto crossbow = static_cast<CrossbowMan*>(object);
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1));

    auto transform = object->GetComponent<TransformComponent>();
    _float4x4 boneMat = animator->GetCombinedMatrices()[crossbow->m_iMuzzleBoneIndex];
    _float4x4 worldMat = transform->GetWorldMatrix();
    XMStoreFloat4x4(&worldMat, XMLoadFloat4x4(&boneMat) * XMLoadFloat4x4(&worldMat));

    _vector scaleV, positionV, rotationV;
    _float3 position{};
    _float3 dir = transform->GetForward();
    XMMatrixDecompose(&scaleV, &rotationV, &positionV, XMLoadFloat4x4(&worldMat));
    XMStoreFloat3(&position, positionV);

    Object* defaultBullet = nullptr;
    Object::OBJECT_DESC desc{};
    desc.scale = _float3{ 3.f,3.f,3.f };
    desc.position = position;
    engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Default_Bullet", engine->GetCurrLevelID(), "Layer_Projectile", &desc, &defaultBullet);
    
    defaultBullet->GetComponent<TransformComponent>()->SetForward(dir);
}

void CrossbowMan::CrossbowManFire::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManFire::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManReload);
    }
}

void CrossbowMan::CrossbowManReload::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Reload));
}

void CrossbowMan::CrossbowManReload::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManReload::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManHitBody::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitBody),false,true);
}

void CrossbowMan::CrossbowManHitBody::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManHitBody::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}

void CrossbowMan::CrossbowManHitHead::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::HitHead), false, true);
}

void CrossbowMan::CrossbowManHitHead::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManHitHead::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManIdle);
    }
}


void CrossbowMan::CrossbowManDead::Enter(Object* object)
{
    object->SetDead();

    auto engine = EngineCore::GetInstance();
    auto random = engine->GetRandom();
    auto transform = object->GetComponent<TransformComponent>();

    /*spawn fracture*/
    {
        _float3 camPosition = engine->GetCameraContext().camPosition;
        _float3 position = transform->GetPosition();
        _float3 hitDir{};
        XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&camPosition)));

        Fracture::FRACTURE_DESC desc{};
        desc.scale = _float3{ 1.f,1.f,1.f };
        desc.quaternion = transform->GetQuaternion();

        for (_uint i = 0; i < 12; i += 2)
        {
            desc.position.x = position.x + random->get<_float>(-4.f, 4.f);
            desc.position.y = position.y + random->get<_float>(4.f, 7.f);
            desc.position.z = position.z + random->get<_float>(-4.f, 4.f);

            _float3 dir{};
            _float dirFactor = random->get<_float>(0.4f, 0.6f);
            XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&desc.position) - XMLoadFloat3(&position)));
            XMStoreFloat3(&dir, XMVector3Normalize((XMLoadFloat3(&hitDir) * dirFactor + XMLoadFloat3(&dir) * (1.f - dirFactor))));
            XMStoreFloat3(&dir, XMVector3Normalize((XMLoadFloat3(&dir) + XMVectorSet(0.f, 0.2f, 0.f, 0.f))));

            _float power = random->get<_float>(90.f, 150.f);
            _float3 force{};
            _float3 angularForce{};
            XMStoreFloat3(&force, XMLoadFloat3(&dir) * power);
            XMStoreFloat3(&angularForce, XMLoadFloat3(&dir) * power * 0.1f);

            _string modelTag = "CrossbowMan" + std::to_string(i);
            desc.modelTag = "Model_Fracture_" + modelTag;

            Object* fracture = nullptr;
            desc.spawnNavCell = object->GetComponent<NavigationComponent>()->GetCurrCellIndex();
            engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Fracture", engine->GetCurrLevelID(), "Layer_Fracture", &desc, &fracture);

            fracture->GetComponent<RigidBodyComponent>()->AddImpulse(force);
            fracture->GetComponent<RigidBodyComponent>()->AddAngularImpulse(angularForce);
        }
    }
}

void CrossbowMan::CrossbowManDead::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManDead::TestForExit(Object* object)
{
}


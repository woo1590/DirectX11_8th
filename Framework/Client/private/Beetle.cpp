#include "pch.h"
#include "Beetle.h"
#include "Bounding_AABB.h"
#include "Random.h"
#include "DamageFont.h"
#include "MaterialInstance.h"
#include "DefaultBullet.h"

#include "Fracture.h"
#include "EnemyHpPanel.h"
#include "Socket.h"
#include "Beetle_Head.h"

//component
#include "StatusComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "NavigationComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"

Beetle::Beetle()
    :Enemy()
{
}

Beetle::Beetle(const Beetle& prototype)
    :Enemy(prototype)
{
}

Beetle* Beetle::Create()
{
    Beetle* Instance = new Beetle();

    if (FAILED(Instance->Initialize_Prototype()))
        Safe_Release(Instance);

    return Instance;
}

HRESULT Beetle::Initialize_Prototype()
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

HRESULT Beetle::Initialize(InitDESC* arg)
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
    aabbDesc.center = _float3{ 0.f,2.5f,0.f };
    aabbDesc.halfSize = _float3{ 2.5f,2.5f,2.5f };
    auto collider = GetComponent<ColliderComponent>();
    collider->Initialize(&aabbDesc);
    engine->RegisterCollider(collider);

    /*status*/
    StatusComponent::STATUS_DESC statusDesc{};
    statusDesc.hp = 60;
    auto status = GetComponent<StatusComponent>();
    status->Initialize(&statusDesc);

    /*model*/
    auto model = GetComponent<ModelComponent>();
    model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_Beetle");
    model->Initialize(nullptr);

    /*animator*/
    auto animator = GetComponent<AnimatorComponent>();
    animator->SetAnimation(ENUM_CLASS(LevelID::Static), "AnimationSet_Enemy_Beetle");

    model->ConnectAnimator();

    /*navigation*/
    auto nav = GetComponent<NavigationComponent>();
    engine->RegisterNavigation(nav);
    nav->AttachTransform();
    nav->AttachRigidBody();
    nav->SpawnInCell(3);
    nav->SetMoveSpeed(25.f);
    nav->SetArriveRange(60.f);

    /*outline model*/
    m_pOutLineModel = ModelComponent::Create(this);
    m_pOutLineModel->SetModel(ENUM_CLASS(LevelID::Static), "Model_Enemy_Beetle");
    m_pOutLineModel->Initialize(nullptr);
    auto outlineMtrlInstance = m_pOutLineModel->GetMaterialInstance();
    outlineMtrlInstance->SetPass("OutLine_Pass");
    outlineMtrlInstance->SetFloat4("g_OutLineColor", _float4(0.f, 0.f, 0.f, 1.f));
    outlineMtrlInstance->SetFloat("g_OutLineWidth", 0.1f);

    m_iHpPanelBoneIndex = model->GetBoneIndex("MonsterHp");
    //m_pTransform->SetScale(_float3{ 1.3f,1.3f,1.3f });

    if (FAILED(CreatePartObjects()))
        return E_FAIL;

    return S_OK;
}

HRESULT Beetle::LateInitialize()
{
    __super::LateInitialize();

    ChangeState(&m_BeetleShow);

    return S_OK;
}

void Beetle::PriorityUpdate(_float dt)
{
    __super::PriorityUpdate(dt);
}

void Beetle::Update(_float dt)
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

void Beetle::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);
}

void Beetle::OnCollisionEnter(ColliderComponent* otherCollider)
{
    __super::OnCollisionEnter(otherCollider);

    auto engine = EngineCore::GetInstance();
    auto random = engine->GetRandom();

    switch (static_cast<ColliderFilter>(otherCollider->GetFilter()))
    {
    case ColliderFilter::PlayerAttack:
    {
        auto status = GetComponent<StatusComponent>();
        auto otherStatus = otherCollider->GetOwner()->GetComponent<StatusComponent>();

        status->BeAttacked(otherStatus->GetDesc().attackPower);
        if (0 == status->GetDesc().hp && m_CurrState != &m_BeetleDead)
            ChangeState(&m_BeetleDead);
     
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
        if (0 == status->GetDesc().hp && m_CurrState != &m_BeetleDead)
            ChangeState(&m_BeetleDead);

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

Object* Beetle::Clone(InitDESC* arg)
{
    Beetle* Instance = new Beetle(*this);

    if (FAILED(Instance->Initialize(arg)))
        Safe_Release(Instance);

    return Instance;
}

void Beetle::Free()
{
    EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());
    __super::Free();
}

HRESULT Beetle::CreatePartObjects()
{
    auto engine = EngineCore::GetInstance();

    /*add head socket*/
    {
        Socket::SOCKET_DESC headSocketDesc{};
        headSocketDesc.parent = this;
        headSocketDesc.parentModel = GetComponent<ModelComponent>();
        headSocketDesc.boneIndex = GetComponent<ModelComponent>()->GetBoneIndex("Bip001 Head");
        if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Socket", ENUM_CLASS(Parts::Head_Socket), &headSocketDesc)))
            return E_FAIL;
    }
    /*add head*/
    {
        Beetle_Head::BEETLE_HEAD_DESC headDesc{};
        headDesc.parent = this;
        headDesc.parentSocketTransform = m_PartObjects[ENUM_CLASS(Parts::Head_Socket)]->GetComponent<TransformComponent>();
        if (FAILED(AddPartObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Beetle_Head", ENUM_CLASS(Parts::Head), &headDesc)))
            return E_FAIL;
    }

    return S_OK;
}

void Beetle::BeetleShow::Enter(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);

    auto rigidBody = object->GetComponent<RigidBodyComponent>();
    rigidBody->SetVelocity(_float3{ 0.f,0.f,0.f });

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    EffectContainer::EFFECT_CONTAINER_DESC effectDesc{};
    effectDesc.position = object->GetComponent<TransformComponent>()->GetPosition();
    XMStoreFloat3(&effectDesc.forward, XMVector3Normalize(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_SpawnSmoke", engine->GetCurrLevelID(), "Layer_Effect", &effectDesc);

    m_fElapsedTime = 0.f;
}

void Beetle::BeetleShow::Update(Object* object, _float dt)
{
    m_fElapsedTime += dt;
}

void Beetle::BeetleShow::TestForExit(Object* object)
{
    if (m_fElapsedTime >= m_fDuration)
    {
        auto beetle = static_cast<Beetle*>(object);
        beetle->ChangeState(&beetle->m_BeetleIdle);
    }
}

void Beetle::BeetleIdle::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::StandInCombat), true);
}

void Beetle::BeetleIdle::Update(Object* object, _float dt)
{
}

void Beetle::BeetleIdle::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    if (distance < 500.f)
    {
        auto beetle = static_cast<Beetle*>(object);
        beetle->ChangeState(&beetle->m_BeetleRun);
    }
}

void Beetle::BeetleRun::Enter(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Run), true);
    animator->SetPlaySpeedScale(1.2f);
    
    m_fSoundElpasedTime = 0.f;
}

void Beetle::BeetleRun::Update(Object* object, _float dt)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    auto transform = object->GetComponent<TransformComponent>();
    auto nav = object->GetComponent<NavigationComponent>();

    _uint currCellIndex = nav->GetCurrCellIndex();
    _uint targetCellIndex = player->GetComponent<NavigationComponent>()->GetCurrCellIndex();
    _float3 position = transform->GetPosition();
    _float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();

    nav->FindPath(position, currCellIndex, targetPosition, targetCellIndex);
    nav->MoveByPath(dt);

    m_fSoundElpasedTime += dt;
    if (m_fSoundElpasedTime >= m_fSoundDuration)
    {
        engine->Play3DSound("SFX_BeetleRun", transform->GetPosition(), 0.6f);

        m_fSoundElpasedTime = 0.f;
    }
}

void Beetle::BeetleRun::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    auto beetle = static_cast<Beetle*>(object);
    if (distance >= 500.f)
    {
        beetle->ChangeState(&beetle->m_BeetleIdle);
    }
    else if (distance < 20.f)
    {
        beetle->ChangeState(&beetle->m_BeetleAttack);
    }
}

void Beetle::BeetleAttack::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Attack));
}

void Beetle::BeetleAttack::Update(Object* object, _float dt)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    _float progress = animator->GetProgress();

    if (progress >= 0.4f)
    {
        auto beetle = static_cast<Beetle*>(object);
        beetle->m_PartObjects[ENUM_CLASS(Parts::Head)]->GetComponent<ColliderComponent>()->SetActive(true);
    }
}

void Beetle::BeetleAttack::TestForExit(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();

    if (animator->IsFinished())
    {
        auto beetle = static_cast<Beetle*>(object);
        beetle->ChangeState(&beetle->m_BeetleIdle);
        beetle->m_PartObjects[ENUM_CLASS(Parts::Head)]->GetComponent<ColliderComponent>()->SetActive(false);
    }
}

void Beetle::BeetleDead::Enter(Object* object)
{
    object->SetDead();

    auto engine = EngineCore::GetInstance();
    auto transform = object->GetComponent<TransformComponent>();

    _float3 camPosition = engine->GetCameraContext().camPosition;
    _float3 position = transform->GetPosition();
    _float3 hitDir{};
    XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&camPosition)));

    Fracture::FRACTURE_DESC desc{};
    desc.scale = _float3{ 1.2f,1.2f,1.2f };
    desc.quaternion = transform->GetQuaternion();

    auto random = engine->GetRandom();
    for (_uint i = 0; i < 2; ++i)
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

        _string modelTag = "Beetle" + std::to_string(i);
        desc.modelTag = "Model_Fracture_" + modelTag;

        Object* fracture = nullptr;
        desc.spawnNavCell = object->GetComponent<NavigationComponent>()->GetCurrCellIndex();
        engine->AddObject(ENUM_CLASS(LevelID::Static), "Prototype_Object_Fracture", engine->GetCurrLevelID(), "Layer_Fracture", &desc, &fracture);

        fracture->GetComponent<RigidBodyComponent>()->AddImpulse(force);
        fracture->GetComponent<RigidBodyComponent>()->AddAngularImpulse(angularForce);
    }
}

void Beetle::BeetleDead::Update(Object* object, _float dt)
{
}

void Beetle::BeetleDead::TestForExit(Object* object)
{
}

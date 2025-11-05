#include "pch.h"
#include "CrossbowMan.h"
#include "Bounding_AABB.h"
#include "Random.h"

#include "Fracture.h"

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
    nav->SetMoveSpeed(15.f);
    nav->SetArriveRange(60.f);

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

    if (EngineCore::GetInstance()->IsKeyPressed('1'))
        ChangeState(&m_CrossbowManDead);
}

void CrossbowMan::LateUpdate(_float dt)
{
    __super::LateUpdate(dt);
}

void CrossbowMan::OnCollisionEnter(ColliderComponent* otherCollider)
{

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
}

void CrossbowMan::CrossbowManIdle::TestForExit(Object* object)
{
    auto engine = EngineCore::GetInstance();

    auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");
    _float3 position = object->GetComponent<TransformComponent>()->GetPosition();
    _float3 playerPos = player->GetComponent<TransformComponent>()->GetPosition();

    _float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerPos) - XMLoadFloat3(&position)));

    if (distance < 1000.f)
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManWalk_F);
    }
}

void CrossbowMan::CrossbowManWalk_F::Enter(Object* object)
{
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Walk_F), true);
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

    if (distance < 65.f)
    {
        auto crossbow = static_cast<CrossbowMan*>(object);
        crossbow->ChangeState(&crossbow->m_CrossbowManFire);
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
    auto animator = object->GetComponent<AnimatorComponent>();
    animator->ChangeAnimation(ENUM_CLASS(AnimationState::Fire1));
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

void CrossbowMan::CrossbowManDead::Enter(Object* object)
{
    object->SetDead();

    auto engine = EngineCore::GetInstance();
    auto transform = object->GetComponent<TransformComponent>();

    _float3 camPosition = engine->GetCameraContext().camPosition;
    _float3 position = transform->GetPosition();
    _float3 hitDir{};
    XMStoreFloat3(&hitDir, XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&camPosition)));

    Fracture::FRACTURE_DESC desc{};
    desc.scale = _float3{ 1.f,1.f,1.f };
    desc.quaternion = transform->GetQuaternion();

    auto random = engine->GetRandom();
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

void CrossbowMan::CrossbowManDead::Update(Object* object, _float dt)
{
}

void CrossbowMan::CrossbowManDead::TestForExit(Object* object)
{
}

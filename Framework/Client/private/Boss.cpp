#include "pch.h"
#include "Boss.h"
#include "Bounding_AABB.h"

//component	
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "NavigationComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

Boss::Boss()
	:Enemy()
{
}

Boss::Boss(const Boss& prototype)
	:Enemy(prototype)
{
}

Boss::~Boss()
{
}

Boss* Boss::Create()
{
	Boss* Instance = new Boss();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Boss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<ColliderComponent>();

	m_strInstanceTag = "Boss";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT Boss::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	Bounding_AABB::AABB_DESC aabbDesc{};
	aabbDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Enemy);
	aabbDesc.type = ColliderType::AABB;
	aabbDesc.center = _float3{ 0.f,5.f,0.f };
	aabbDesc.halfSize = _float3{ 6.f,10.f,6.f };
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&aabbDesc);

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Enemy_Boss");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Enemy_Boss");

	model->ConnectAnimator();
	animator->ChangeAnimation(1, true);

	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachTransform();
	nav->AttachRigidBody();
	nav->SpawnInCell(0);

	return S_OK;
}

void Boss::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Boss::Update(_float dt)
{
	__super::Update(dt);
}

void Boss::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* Boss::Clone(InitDESC* arg)
{
	Boss* Instance = new Boss(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Boss::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

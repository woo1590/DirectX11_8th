#include "pch.h"
#include "Dumpling.h"
#include "Bounding_Sphere.h"

//component
#include "RigidBodyComponent.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"

Dumpling::Dumpling()
	:Item()
{
}

Dumpling::Dumpling(const Dumpling& prototype)
	:Item(prototype)
{
}

Dumpling* Dumpling::Create()
{
	Dumpling* Instance = new Dumpling();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Dumpling::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<RigidBodyComponent>();

	return S_OK;
}

HRESULT Dumpling::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Item_Dumpling");

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Item);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 2.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*rigid body*/
	RigidBodyComponent::RIGID_BODY_DESC rigidBodyDesc{};
	rigidBodyDesc.angularDamping = 0.01f;
	rigidBodyDesc.linearDamping = 0.1f;
	rigidBodyDesc.mass = 1.f;

	auto rigidBody = GetComponent<RigidBodyComponent>();
	rigidBody->Initialize(&rigidBodyDesc);
	rigidBody->SetGround(false);
	rigidBody->UseIntegrate();
	rigidBody->SetInertiaTensor(1.5f);

	m_pTransform->SetScale(_float3{ 1.2f,1.2f,1.2f });
	ChangeState(&m_DumplingSpawn);

	return S_OK;
}

void Dumpling::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Dumpling::Update(_float dt)
{
	__super::Update(dt);
}

void Dumpling::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void Dumpling::Interaction(PlayerInteractionComponent* interaction)
{
	SetDead();
}

Object* Dumpling::Clone(InitDESC* arg)
{
	Dumpling* Instance = new Dumpling(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Dumpling::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

void Dumpling::DumplingSpawn::Enter(Object* object)
{
	m_fElapsedTime = 0.f;
}

void Dumpling::DumplingSpawn::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	auto engine = EngineCore::GetInstance();
	auto rigidBody = object->GetComponent<RigidBodyComponent>();
	auto transform = object->GetComponent<TransformComponent>();

	_float3 velocity = rigidBody->GetVelocity();
	_float3x3 invInertiaTensor = rigidBody->GetInvInertiaTensor();

	_float3 currPosition = transform->GetPosition();
	_float3 dir{};
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&velocity)));

	RAY worldRay{};
	worldRay.origin = currPosition;
	worldRay.direction = dir;

	RAYCAST_DATA data = engine->RayCast(worldRay, 2.f, ENUM_CLASS(ColliderFilter::StaticMapObject));

	if (data.isHit)
	{
		_float3 outVel{};
		_float3 hitPosition{};

		XMStoreFloat3(&hitPosition, XMLoadFloat3(&currPosition) * data.worldDistance + XMLoadFloat3(&worldRay.direction));
		_float t = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&velocity) * -1.f, XMLoadFloat3(&data.hitNormal)));

		/*make linear velocity*/
		_vector reflectVec = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		reflectVec = XMLoadFloat3(&velocity) + 2.f * t * XMLoadFloat3(&data.hitNormal) * 0.8f;

		if (XMVectorGetX(XMVector3Length(reflectVec)) < 0.3f)
			reflectVec = XMVectorZero();

		XMStoreFloat3(&velocity, reflectVec);
	}

	rigidBody->SetVelocity(velocity);
	transform->Translate(XMLoadFloat3(&velocity) * dt);
}

void Dumpling::DumplingSpawn::TestForExit(Object* object)
{
	if (m_fElapsedTime >= m_fDuration)
	{
		auto dumpling = static_cast<Dumpling*>(object);
		dumpling->ChangeState(&dumpling->m_DumplingIdle);
	}
}

void Dumpling::DumplingIdle::Enter(Object* object)
{
	m_CurrPosition = object->GetComponent<TransformComponent>()->GetPosition();
	m_fElapsedTime = 0.f;
}

void Dumpling::DumplingIdle::Update(Object* object, _float dt)
{
	m_fElapsedTime += dt;

	auto transform = object->GetComponent<TransformComponent>();

	_float3 hoverPosition{};
	hoverPosition = m_CurrPosition;
	hoverPosition.y = m_CurrPosition.y + std::sinf(m_fElapsedTime * 2.f);

	transform->SetPosition(hoverPosition);
}

void Dumpling::DumplingIdle::TestForExit(Object* object)
{
	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");

	_float3 currPosition = transform->GetPosition();
	_float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();
	_vector dir = XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition);
	_float distance = XMVectorGetX(dir);

	if (distance < m_fMagneticDistance)
	{
		auto dumpling = static_cast<Dumpling*>(object);
		dumpling->ChangeState(&dumpling->m_DumplingMagnetic);
	}
}

void Dumpling::DumplingMagnetic::Enter(Object* object)
{
}

void Dumpling::DumplingMagnetic::Update(Object* object, _float dt)
{
	auto engine = EngineCore::GetInstance();
	auto transform = object->GetComponent<TransformComponent>();
	auto player = engine->GetFrontObject(ENUM_CLASS(LevelID::Static), "Layer_Player");

	_float3 currPosition = transform->GetPosition();
	_float3 targetPosition = player->GetComponent<TransformComponent>()->GetPosition();
	_vector dir = XMLoadFloat3(&targetPosition) - XMLoadFloat3(&currPosition);
	_float distance = XMVectorGetX(XMVector3Length(dir));

	_float t = distance / m_fMagneticDistance;
	t = std::clamp(t, 0.f, 1.f);
	t = 1.f - t;

	transform->Translate(dir * t * m_fSpeed * dt);
}

void Dumpling::DumplingMagnetic::TestForExit(Object* object)
{
}

#include "pch.h"
#include "Fracture.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "NavigationComponent.h"

Fracture::Fracture()
	:Object()
{
}

Fracture::Fracture(const Fracture& prototype)
	:Object(prototype)
{
}

Fracture* Fracture::Create()
{
	Fracture* Instance = new Fracture();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Fracture::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<RigidBodyComponent>();
	AddComponent<NavigationComponent>();
	AddComponent<ColliderComponent>();

	return S_OK;
}

HRESULT Fracture::Initialize(InitDESC* arg)
{
	FRACTURE_DESC* desc = static_cast<FRACTURE_DESC*>(arg);

	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::Fracture);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 1.5f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*model*/	
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), desc->modelTag);

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

	/*navigation*/
	auto nav = GetComponent<NavigationComponent>();
	engine->RegisterNavigation(nav);
	nav->AttachRigidBody();
	nav->AttachTransform();
	nav->SetCurrCellIndex(desc->spawnNavCell);

	return S_OK;
}

void Fracture::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void Fracture::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;

	if (m_fElapsedTime >= m_fLifeDuration)
		SetDead();

	auto engine = EngineCore::GetInstance();
	auto rigidBody = GetComponent<RigidBodyComponent>();

	_float3 velocity = rigidBody->GetVelocity();
	_float3 angularVelocity = rigidBody->GetAngularVelocity();
	_float3x3 invInertiaTensor = rigidBody->GetInvInertiaTensor();

	_float3 currPosition = m_pTransform->GetPosition();
	_float3 dir{};
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&velocity)));

	RAY worldRay{};
	worldRay.origin = currPosition;
	worldRay.direction = dir;

	RAYCAST_DATA data = engine->RayCast(worldRay, 2.f, ENUM_CLASS(ColliderFilter::Ray));

	if (data.isHit)
	{
		_float3 outVel{};
		_float3 hitPosition{};

		XMStoreFloat3(&hitPosition, XMLoadFloat3(&currPosition) * data.worldDistance + XMLoadFloat3(&worldRay.direction));
		_float t = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&velocity) * -1.f, XMLoadFloat3(&data.hitNormal)));
		
		/*make linear velocity*/
		_vector slideVec{};
		_vector restitutionVec{};
		_vector angularVec{};

		slideVec = XMLoadFloat3(&velocity) + t * XMLoadFloat3(&data.hitNormal);
		restitutionVec = XMLoadFloat3(&data.hitNormal) * 0.5f * t;

		if (XMVectorGetX(XMVector3Length(slideVec)) < 1.f)
			slideVec = XMVectorZero();

		if (XMVectorGetX(XMVector3Length(restitutionVec)) < 0.1f)
			restitutionVec = XMVectorZero();

		angularVec = XMVector3TransformNormal(slideVec * 0.3f, XMLoadFloat3x3(&invInertiaTensor));
		XMStoreFloat3(&angularVelocity, angularVec);

		XMStoreFloat3(&velocity, slideVec + restitutionVec);
	}
	
	rigidBody->SetVelocity(velocity);
	rigidBody->SetAngularVelocity(angularVelocity);
	m_pTransform->Translate(XMLoadFloat3(&velocity) * dt);
}

void Fracture::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	
}

Object* Fracture::Clone(InitDESC* arg)
{
	Fracture* Instance = new Fracture(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void Fracture::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

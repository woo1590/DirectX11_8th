#include "pch.h"
#include "DefaultBullet.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "StatusComponent.h"

DefaultBullet::DefaultBullet()
	:Projectile()
{
}

DefaultBullet::DefaultBullet(const DefaultBullet& prototype)
	:Projectile(prototype)
{
}

DefaultBullet* DefaultBullet::Create()
{
	DefaultBullet* Instance = new DefaultBullet();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DefaultBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();
	AddComponent<StatusComponent>();

	m_strInstanceTag = "DefaultBullet";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT DefaultBullet::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::PlayerProjectile);
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 0.3f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	EngineCore::GetInstance()->RegisterCollider(collider);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::Static), "Model_Projectile_Default_Bullet");

	/*status*/
	auto status = GetComponent<StatusComponent>();
	StatusComponent::STATUS_DESC statusDesc{};
	statusDesc.attackPower = 20;
	status->Initialize(&statusDesc);

	m_fSpeed = 1000.f;

	return S_OK;
}

void DefaultBullet::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DefaultBullet::Update(_float dt)
{
	__super::Update(dt);

	_float3 forward = m_pTransform->GetForward();
	_vector velocity = XMLoadFloat3(&forward) * m_fSpeed;

	_float3 currPosition = m_pTransform->GetPosition();
	_float3 nextPosition{};
	XMStoreFloat3(&nextPosition, XMLoadFloat3(&currPosition) + velocity * dt);

	RAY worldRay{};
	_float maxDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));
	worldRay.origin = currPosition;
	XMStoreFloat3(&worldRay.direction, XMVector3Normalize(XMLoadFloat3(&nextPosition) - XMLoadFloat3(&currPosition)));

	RAYCAST_DATA result = EngineCore::GetInstance()->RayCast(worldRay, maxDistance, ENUM_CLASS(ColliderFilter::StaticMapObject));
	if (result.isHit)
	{
		//currPos + result.distance 더해서 nextPos 만들고 nextPos에 데칼 생성
		SetDead();
	}
	else
		m_pTransform->SetPosition(nextPosition);
}

void DefaultBullet::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

void DefaultBullet::OnCollisionEnter(ColliderComponent* otherCollider)
{
	SetDead();
}

Object* DefaultBullet::Clone(InitDESC* arg)
{
	DefaultBullet* Instance = new DefaultBullet(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void DefaultBullet::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());

	__super::Free();
}

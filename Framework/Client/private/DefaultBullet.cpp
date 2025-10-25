#include "pch.h"
#include "DefaultBullet.h"
#include "Bounding_Sphere.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"

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

	m_strInstanceTag = "DefaultBullet";
	m_eRenderGroup = RenderGroup::NonBlend;

	return S_OK;
}

HRESULT DefaultBullet::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.radius = 0.3f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Projectile_Default_Bullet");

	return S_OK;
}

void DefaultBullet::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void DefaultBullet::Update(_float dt)
{
	__super::Update(dt);

	_float speed = 1000.f;
	_float3 forward = m_pTransform->GetForward();
	_vector velocity = XMLoadFloat3(&forward) * speed;

	m_pTransform->Translate(velocity * dt);

	m_fElapsedTime += dt;
}

void DefaultBullet::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);

	if (m_fElapsedTime > m_fDuration)
		SetDead();

	auto engine = EngineCore::GetInstance();

	auto collider = GetComponent<ColliderComponent>();
	auto& enemies = engine->GetObjects(ENUM_CLASS(LevelID::GamePlay), "Layer_Enemy");
	for (const auto& enemy : enemies)
	{
		auto enemyCollider = enemy->GetComponent<ColliderComponent>();
		if (collider->Intersect(enemyCollider))
		{
			enemy->SetDead();
			SetDead();
		}
	}

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
	__super::Free();
}

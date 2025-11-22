#include "pch.h"
#include "BossEye.h"
#include "Bounding_Sphere.h"
#include "MaterialInstance.h"

//component
#include "ModelComponent.h"
#include "ColliderComponent.h"

BossEye::BossEye()
	:PartObject()
{
}

BossEye::BossEye(const BossEye& prototype)
	:PartObject(prototype)
{
}

BossEye* BossEye::Create()
{
	BossEye* Instance = new BossEye();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BossEye::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	AddComponent<ColliderComponent>();

	m_eRenderGroup = RenderGroup::Blend;
	m_strInstanceTag = "BossEye";

	return S_OK;
}

HRESULT BossEye::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();
	BOSS_EYE_DESC* desc = static_cast<BOSS_EYE_DESC*>(arg);

	/*model*/
	auto model = GetComponent<ModelComponent>();
	model->Initialize(nullptr);

	model->SetModel(ENUM_CLASS(LevelID::StageBoss), "Model_BossEye");
	model->GetMaterialInstance()->SetPass("BossEye_Pass");

	/*collider*/
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.colliderFilter = 0;
	sphereDesc.radius = 1.f;
	
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);

	m_pTransform->SetScale(_float3{ 0.04f,0.04f,0.04f });

	if (!desc->isRightEye)
	{
		m_pTransform->Rotate(_float3{ math::ToRadian(-90.f),math::ToRadian(-90.f),0.f });
	}
	else
	{
		m_pTransform->Rotate(_float3{ math::ToRadian(-90.f),math::ToRadian(-90.f),0.f });
		m_pTransform->SetPosition(_float3{ -0.2f,0.f,0.4f });
	}

	return S_OK;
}

void BossEye::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BossEye::Update(_float dt)
{
	__super::Update(dt);
}

void BossEye::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BossEye::Clone(InitDESC* arg)
{
	BossEye* Instance = new BossEye(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BossEye::Free()
{
	__super::Free();
}

#include "pch.h"
#include "BarrelBoom.h"
#include "Bounding_Sphere.h"

//component
#include "ColliderComponent.h"
#include "LightComponent.h"

BarrelBoom::BarrelBoom()
	:Object()
{
}

BarrelBoom::BarrelBoom(const BarrelBoom& prototype)
	:Object(prototype)
{
}

BarrelBoom* BarrelBoom::Create()
{
	BarrelBoom* Instance = new BarrelBoom();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT BarrelBoom::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ColliderComponent>();
	AddComponent<LightComponent>();

	return S_OK;
}

HRESULT BarrelBoom::Initialize(InitDESC* arg)
{
	if(FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto engine = EngineCore::GetInstance();

	/*collider*/	
	Bounding_Sphere::SPHERE_DESC sphereDesc{};
	sphereDesc.type = ColliderType::Sphere;
	sphereDesc.colliderFilter = ENUM_CLASS(ColliderFilter::BarrelBoom);
	sphereDesc.radius = 30.f;
	auto collider = GetComponent<ColliderComponent>();
	collider->Initialize(&sphereDesc);
	engine->RegisterCollider(collider);

	/*light*/
	LightComponent::LIGHT_DESC lightDesc{};
	lightDesc.color = _float4{ 1.f,0.8f,0.8f,1.f };
	lightDesc.type = LightType::Point;
	lightDesc.range = 600.f;
	auto light = GetComponent<LightComponent>();
	light->Initialize(&lightDesc);

	//engine->RegisterLight(light);

	return S_OK;
}

void BarrelBoom::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void BarrelBoom::Update(_float dt)
{
	__super::Update(dt);

	m_fElapsedTime += dt;
	if (m_fElapsedTime >= m_fDuration)
		SetDead();
	else
	{
		auto light = GetComponent<LightComponent>();

		_float t = m_fElapsedTime / m_fDuration;
		t = math::EaseOutSline(t);

		_float range = math::Lerp(600.f, 30.f, t);
		_float4 color{};
		XMStoreFloat4(&color, XMVectorLerp(XMLoadFloat4(&m_StartLightColor), XMLoadFloat4(&m_EndLightColor), t));
		light->SetLightRange(range);
		light->SetColor(color);
	}
}

void BarrelBoom::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* BarrelBoom::Clone(InitDESC* arg)
{
	BarrelBoom* Instance = new BarrelBoom(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void BarrelBoom::Free()
{
	EngineCore::GetInstance()->UnRegisterCollider(GetComponent<ColliderComponent>());
	//EngineCore::GetInstance()->UnRegisterLight(GetComponent<LightComponent>());

	__super::Free();
}

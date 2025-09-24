#include "pch.h"
#include "TestObject.h"
#include "Material.h"
#include "Random.h"
#include "PartObject.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "AudioSource.h"
#include "CameraComponent.h"

TestObject::TestObject()
{
}

TestObject* TestObject::Create()
{
	TestObject* Instance = new TestObject();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_strInstanceTag = "TestObject";

	AddComponent<AudioSource>();

	return S_OK;
}

HRESULT TestObject::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	if (FAILED(CreatePartObjects()))
		return E_FAIL;

	auto random = EngineCore::GetInstance()->GetRandom();
	m_pTransform->SetPosition(_float3(random->get<_float>(-10.f, 10.f), 0.f,random->get<_float>(-15.f,10.f)));
	
	return S_OK;
}

void TestObject::Free()
{
	__super::Free();

}

HRESULT TestObject::CreatePartObjects()
{
	PartObject::PART_OBJECT_DESC bodyDesc{};
	bodyDesc.parent = this;
	if (FAILED(AddPartObject(ENUM_CLASS(LevelID::GamePlay), "Prototype_Object_TestObjectBody", ENUM_CLASS(Parts::Body), &bodyDesc)))
		return E_FAIL;

	return S_OK;
}

void TestObject::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void TestObject::Update(_float dt)
{
	__super::Update(dt);

	auto engine = EngineCore::GetInstance();

	_vector forward = m_pTransform->GetForwardV();
	_vector right = m_pTransform->GetRightV();
	_float speed = 5.f;
	_vector velocity = forward * speed;

	_float3 rotation = m_pTransform->GetRotation();

	if (engine->IsKeyDown(VK_UP))
		m_pTransform->Translate(forward * speed * dt);
	if (engine->IsKeyDown(VK_DOWN))
		m_pTransform->Translate(-forward * speed * dt);
	if (engine->IsKeyDown(VK_LEFT))
	{
		rotation.y -= speed * dt;
		m_pTransform->SetRotation(rotation);
	}
	if (engine->IsKeyDown(VK_RIGHT))
	{
		rotation.y += speed * dt;
		m_pTransform->SetRotation(rotation);
	}

}

void TestObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT TestObject::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	for (const auto& part : m_PartObjects)
	{
		if (part)
		{
			if (FAILED(part->ExtractRenderProxies(proxies)))
				return E_FAIL;
		}
	}

	return S_OK;
}

Object* TestObject::Clone(InitDESC* arg)
{
	TestObject* Instance = new TestObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

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

	AddComponent<ModelComponent>();
	AddComponent<AnimatorComponent>();
	AddComponent<AudioSource>();

	return S_OK;
}

HRESULT TestObject::Initialize(InitDESC* arg)
{
	ContainerObject::CONTAINER_OBJECT_DESC desc{};
	desc.numPartObjects = ENUM_CLASS(Parts::Count);

	if (FAILED(__super::Initialize(&desc)))
		return E_FAIL;

	auto random = EngineCore::GetInstance()->GetRandom();
	
	m_pTransform->SetPosition(_float3(random->get<_float>(-10.f, 10.f), 0.f,random->get<_float>(-15.f,10.f)));

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Test");

	auto animator = GetComponent<AnimatorComponent>();
	animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Test");

	model->ConnectAnimator();
	animator->ChangeAnimation(random->get<_int>(0,3),true);
	
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

}

void TestObject::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* TestObject::Clone(InitDESC* arg)
{
	TestObject* Instance = new TestObject(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

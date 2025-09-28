#include "pch.h"
#include "TestObjectBody.h"
#include "ContainerObject.h"

//component
#include "ModelComponent.h"
#include "AnimatorComponent.h"

TestObjectBody::TestObjectBody()
	:PartObject()
{
}

TestObjectBody::TestObjectBody(const TestObjectBody& prototype)
	:PartObject(prototype)
{
}

TestObjectBody* TestObjectBody::Create()
{
	TestObjectBody* Instance = new TestObjectBody();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestObjectBody::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	AddComponent<ModelComponent>();
	//AddComponent<AnimatorComponent>();

	return S_OK;
}

HRESULT TestObjectBody::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	auto model = GetComponent<ModelComponent>();
	model->SetModel(ENUM_CLASS(LevelID::GamePlay), "Model_Stage1_Wall0");

	//auto animator = GetComponent<AnimatorComponent>();
	//animator->SetAnimation(ENUM_CLASS(LevelID::GamePlay), "AnimationSet_Test");

	return S_OK;
}

void TestObjectBody::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void TestObjectBody::Update(_float dt)
{
	__super::Update(dt);

}

void TestObjectBody::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT TestObjectBody::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	auto model = GetComponent<ModelComponent>();
	if (!model)
		return S_OK;

	return model->ExtractRenderProxy(m_pTransform, proxies[ENUM_CLASS(RenderGroup::NonBlend)]);
}

Object* TestObjectBody::Clone(InitDESC* arg)
{
	TestObjectBody* Instance = new TestObjectBody(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

void TestObjectBody::Free()
{
	__super::Free();
}

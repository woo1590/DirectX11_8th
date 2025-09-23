#include "pch.h"
#include "TestObjectBody.h"

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

	return S_OK;
}

HRESULT TestObjectBody::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

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

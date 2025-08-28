#include "pch.h"
#include "TestCube.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

TestCube::TestCube()
{
}

TestCube* TestCube::Create()
{
	TestCube* Instance = new TestCube();

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestCube::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT TestCube::Initialize(InitDESC* arg)
{
	if (FAILED(__super::Initialize(arg)))
		return E_FAIL;

	TransformComponent::TransformDESC transformDesc{};
	transformDesc.position = _float3(1.f, 1.f, 1.f);

	if (FAILED(GetComponent<TransformComponent>()->Initialize(&transformDesc)))
		return E_FAIL;

	return S_OK;
}

void TestCube::Free()
{
	__super::Free();
}

void TestCube::Update(_float dt)
{
	__super::Update(dt);
}

void TestCube::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

Object* TestCube::Clone(InitDESC* arg)
{
	TestCube* Instance = new TestCube(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

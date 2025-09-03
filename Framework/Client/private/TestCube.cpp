#include "pch.h"
#include "TestCube.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Material.h"

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

	return S_OK;
}

void TestCube::Free()
{
	__super::Free();

	Safe_Release(m_pMaterial);
}

void TestCube::PriorityUpdate(_float dt)
{
	__super::PriorityUpdate(dt);
}

void TestCube::Update(_float dt)
{
	__super::Update(dt);

}

void TestCube::LateUpdate(_float dt)
{
	__super::LateUpdate(dt);
}

HRESULT TestCube::ExtractRenderProxies(std::vector<std::vector<RenderProxy>>& proxies)
{
	return S_OK;
}

Object* TestCube::Clone(InitDESC* arg)
{
	TestCube* Instance = new TestCube(*this);

	if (FAILED(Instance->Initialize(arg)))
		Safe_Release(Instance);

	return Instance;
}

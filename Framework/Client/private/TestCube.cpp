#include "pch.h"
#include "TestCube.h"

TestCube::TestCube()
{
}

TestCube* TestCube::Create()
{
	TestCube* Instance = new TestCube();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestCube::Initialize()
{
	Object::Initialize();

	return S_OK;
}

void TestCube::Free()
{
	Object::Free();
}

void TestCube::Update(_float dt)
{
	Object::Update(dt);
}

void TestCube::LateUpdate(_float dt)
{
	Object::LateUpdate(dt);
}

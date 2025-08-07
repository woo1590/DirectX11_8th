#include "pch.h"
#include "TestLevel.h"

TestLevel::TestLevel()
{
}

TestLevel* TestLevel::Create()
{
	TestLevel* Instance = new TestLevel();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT TestLevel::Initialize()
{
	return S_OK;
}

void TestLevel::Free()
{
	__super::Free();
}

#include "EnginePCH.h"
#include "Model.h"

Model::Model()
{
}

Model* Model::Create()
{
	Model* Instance = new Model();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Model::Initialize()
{
	return S_OK;
}

void Model::Free()
{
	__super::Free();
}

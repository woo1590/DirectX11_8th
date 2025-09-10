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

HRESULT Model::ExtractRenderProxy(RenderProxy& proxy)
{
	proxy.buffer = m_pRootNode->bufferMaterial.first;
	proxy.material = m_pRootNode->bufferMaterial.second;
	proxy.frameIndex = 0;

	return S_OK;
}


void Model::Free()
{
	__super::Free();
}

#include "EnginePCH.h"
#include "Renderer.h"
#include "VIBuffer.h"

Renderer::Renderer(ID3D11DeviceContext* pDeviceContext)
	:m_pDeviceContext(pDeviceContext)
{
	pDeviceContext->AddRef();
}

Renderer* Renderer::Create(ID3D11DeviceContext* pDeviceContext)
{
	Renderer* Instance = new Renderer(pDeviceContext);
	
	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Renderer::Initialize()
{
	return S_OK;
}

_int Renderer::UploadBuffer(VIBuffer* pBuffer)
{
	GPUBufferData data{};

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	
	return 1;
}

void Renderer::Free()
{
	__super::Free();

	Safe_Release(m_pDeviceContext);
}

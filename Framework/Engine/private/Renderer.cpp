#include "EnginePCH.h"
#include "Renderer.h"
#include "VIBuffer.h"

Renderer::Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice),
	m_pDeviceContext(pDeviceContext)
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

Renderer* Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	Renderer* Instance = new Renderer(pDevice,pDeviceContext);
	
	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Renderer::Initialize()
{
	return S_OK;
}

void Renderer::Free()
{
	__super::Free();

}

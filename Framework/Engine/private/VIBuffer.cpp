#include "EnginePCH.h"
#include "VIBuffer.h"
#include "EngineCore.h"

VIBuffer::VIBuffer()
	:m_pDevice(EngineCore::GetInstance()->GetDevice())
	,m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

HRESULT VIBuffer::Initialize()
{
	return S_OK;
}

void VIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

HRESULT VIBuffer::BindBuffers()
{
	ID3D11Buffer* vertexBuffers[] =
	{
		m_pVB
	};

	_uint vertexStrides[] =
	{
		m_iVertexStride
	};

	_uint vertexOffsets[] =
	{
		0
	};
	
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, vertexBuffers, vertexStrides, vertexOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);
	
	return S_OK;
}

HRESULT VIBuffer::Render()
{
	m_pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

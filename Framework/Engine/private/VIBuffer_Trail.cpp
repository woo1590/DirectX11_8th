#include "EnginePCH.h"
#include "VIBuffer_Trail.h"

VIBuffer_Trail::VIBuffer_Trail()
	:VIBuffer()
{
}

VIBuffer_Trail* VIBuffer_Trail::Create(_uint numMaxPoints)
{
	VIBuffer_Trail* Instance = new VIBuffer_Trail();

	if (FAILED(Instance->Initialize(numMaxPoints)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBuffer_Trail::Initialize(_uint numMaxPoints)
{
	_uint maxLines = numMaxPoints - 1;

	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTX_RIBBON);
	m_iNumVertices = maxLines * 2;

	m_iIndexStride = 0;
	m_iNumIndices = 0;

	m_eIndexFormat = DXGI_FORMAT_UNKNOWN;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	/*----Vertex buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, nullptr, &m_pVB)))
		return E_FAIL;

	
	return S_OK;
}

void VIBuffer_Trail::UpdateBuffer(const VTX_TRAIL* vertices, _uint numPoints)
{
	_uint numLines = numPoints - 1;

	m_iNumVertices = numLines * 2;

	D3D11_MAPPED_SUBRESOURCE subResource{};

	m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy_s(subResource.pData, sizeof(VTX_TRAIL) * m_iNumVertices, vertices, sizeof(VTX_TRAIL) * m_iNumVertices);
	m_pDeviceContext->Unmap(m_pVB, 0);

}

HRESULT VIBuffer_Trail::BindBuffers()
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
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT VIBuffer_Trail::Draw()
{
	m_pDeviceContext->Draw(m_iNumVertices, 0);

	return S_OK;
}

void VIBuffer_Trail::Free()
{
	__super::Free();
}

#include "EnginePCH.h"
#include "VIBuffer_Point.h"

VIBuffer_Point::VIBuffer_Point()
	:VIBuffer()
{
}

VIBuffer_Point* VIBuffer_Point::Create()
{
	VIBuffer_Point* Instance = new VIBuffer_Point();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBuffer_Point::Initialize()
{
	m_iNumVertexBuffers = 1;

	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOS);

	m_iNumIndices = 0;
	m_iIndexStride = 0;
	m_eIndexFormat = DXGI_FORMAT_UNKNOWN;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	VTXPOS vertex{};
	vertex.position = _float3{ 0.f,0.f,0.f };

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = &vertex;
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT VIBuffer_Point::BindBuffers()
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

HRESULT VIBuffer_Point::Draw()
{
	m_pDeviceContext->Draw(1, 0);

	return S_OK;
}

void VIBuffer_Point::Free()
{
	__super::Free();
}

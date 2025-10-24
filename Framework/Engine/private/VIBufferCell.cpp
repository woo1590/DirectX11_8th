#include "EnginePCH.h"
#include "VIBufferCell.h"

VIBufferCell::VIBufferCell()
	:VIBuffer()
{
}

VIBufferCell* VIBufferCell::Create(const _float3* points)
{
	VIBufferCell* Instance = new VIBufferCell();

	if (FAILED(Instance->Initialize(points)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBufferCell::Initialize(const _float3* points)
{
	m_iNumVertexBuffers = 1;

	m_iNumVertices = 3;
	m_iVertexStride = sizeof(VTXCELL);

	m_iNumIndices = 3;
	m_iIndexStride = 2;

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*---vertex buffer---*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.MiscFlags = 0;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.StructureByteStride = 0;

	std::vector<VTXCELL> vertices(m_iNumVertices);
	vertices[0].position = points[0];
	vertices[1].position = points[1];
	vertices[2].position = points[2];

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	/*---index buffer---*/
	D3D11_BUFFER_DESC ibDesc{};
	ibDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.MiscFlags = 0;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.StructureByteStride = 0;

	std::vector<_ushort> indices(m_iNumIndices);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void VIBufferCell::Free()
{
	__super::Free();
}

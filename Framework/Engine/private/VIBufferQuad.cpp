#include "EnginePCH.h"
#include "VIBufferQuad.h"

VIBufferQuad::VIBufferQuad()
	:VIBuffer()
{
}

VIBufferQuad* VIBufferQuad::Create()
{
	VIBufferQuad* Instance = new VIBufferQuad();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBufferQuad::Initialize()
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXTEX);

	m_iNumIndices = 6;
	m_iIndexStride = 2;

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXTEX> vertices(m_iNumVertices);
	vertices[0].position = _float3(-0.5f, 0.5f, 0.f);
	vertices[0].texCoord = _float2(0.f, 0.f);

	vertices[1].position = _float3(0.5f, 0.5f, 0.f);
	vertices[1].texCoord = _float2(1.f, 0.f);

	vertices[2].position = _float3(0.5f, -0.5f, 0.f);
	vertices[2].texCoord = _float2(1.f, 1.f);

	vertices[3].position = _float3(-0.5f, -0.5f, 0.f);
	vertices[3].texCoord = _float2(0.f, 1.f);

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	/*----Index Buffer----*/
	D3D11_BUFFER_DESC ibDesc{};
	ibDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = m_iIndexStride;

	std::vector<_ushort> indices(m_iNumIndices);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;
	
	return S_OK;
}

void VIBufferQuad::Free()
{
	__super::Free();
}

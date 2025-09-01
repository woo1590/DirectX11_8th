#include "EnginePCH.h"
#include "VIBufferCube.h"

VIBufferCube::VIBufferCube()
	:VIBuffer()
{
}

VIBufferCube* VIBufferCube::Create()
{
	VIBufferCube* Instance = new VIBufferCube();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBufferCube::Initialize()
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXCUBE);

	m_iNumIndices = 36;
	m_iIndexStride = 2;

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*---Vertex Buffer---*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXCUBE> vertices(m_iNumVertices);
	vertices[0].position = _float3(-0.5f, 0.5f, -0.5f);
	vertices[1].position = _float3(0.5f, 0.5f, -0.5f);
	vertices[2].position = _float3(0.5f, -0.5f, -0.5f);
	vertices[3].position = _float3(-0.5f, -0.5f, -0.5f);

	vertices[4].position = _float3(-0.5f, 0.5f, 0.5f);
	vertices[5].position = _float3(0.5f, 0.5f, 0.5f);
	vertices[6].position = _float3(0.5f, -0.5f, 0.5f);
	vertices[7].position = _float3(-0.5f, -0.5f, 0.5f);

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
	/* -Z */
	indices[0] = 0; indices[3] = 0;
	indices[1] = 1; indices[4] = 2;
	indices[2] = 2; indices[5] = 3;

	/* +Z */
	indices[6] = 5; indices[9] =  5;
	indices[7] = 4; indices[10] = 7;
	indices[8] = 7; indices[11] = 6;

	/* -X */
	indices[12] = 4; indices[15] = 4;
	indices[13] = 0; indices[16] = 3;
	indices[14] = 3; indices[17] = 7;

	/* +X */
	indices[18] = 1; indices[21] = 1;
	indices[19] = 5; indices[22] = 6;
	indices[20] = 6; indices[23] = 2;

	/* -Y */
	indices[24] = 3; indices[27] = 3;
	indices[25] = 2; indices[28] = 6;
	indices[26] = 6; indices[29] = 7;

	/* +Y */
	indices[30] = 4; indices[33] = 4;
	indices[31] = 5; indices[34] = 1;
	indices[32] = 1; indices[35] = 0;

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;
	
	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;
	
	return S_OK;
}

void VIBufferCube::Free()
{
	__super::Free();
}

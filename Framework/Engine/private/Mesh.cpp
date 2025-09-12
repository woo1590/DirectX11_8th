#include "EnginePCH.h"
#include "Mesh.h"

Mesh::Mesh()
	:VIBuffer()
{
}

Mesh* Mesh::Create(const MESH_FORMAT& mesh, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices)
{
	Mesh* Instance = new Mesh();

	if (FAILED(Instance->Initialize(mesh, vertices, indices)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Mesh::Initialize(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = meshFormat.numVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = meshFormat.numIndices;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXMESH> vertices(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		vertices[i].position = vertexFormats[i].position;
		vertices[i].normal = vertexFormats[i].normal;
		vertices[i].texCoord = vertexFormats[i].texCoord;
		vertices[i].tangent = vertexFormats[i].tangent;
	}

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

	std::vector<_uint> indices(m_iNumIndices);
	for (_uint i = 0; i < m_iNumIndices; ++i)
		indices[i] = indexFormats[i];

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void Mesh::Free()
{
	__super::Free();
}

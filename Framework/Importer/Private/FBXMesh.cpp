#include "ImporterPCH.h"
#include "FBXMesh.h"

FBXMesh::FBXMesh()
	:VIBuffer()
{
}

FBXMesh* FBXMesh::Create(aiMesh* pMesh)
{
	FBXMesh* Instance = new FBXMesh();

	if (FAILED(Instance->Initialize(pMesh)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXMesh::Initialize(aiMesh* pMesh)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = pMesh->mNumFaces * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_iMaterialIndex = pMesh->mMaterialIndex;

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	m_Vertices.resize(m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy_s(&m_Vertices[i].position, sizeof(_float3), &pMesh->mVertices[i], sizeof(_float3));
		memcpy_s(&m_Vertices[i].normal, sizeof(_float3), &pMesh->mNormals[i], sizeof(_float3));
		memcpy_s(&m_Vertices[i].texCoord, sizeof(_float2), &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy_s(&m_Vertices[i].tangent, sizeof(_float3), &pMesh->mTangents[i], sizeof(_float3));
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = m_Vertices.data();
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

	m_Indices.resize(m_iNumIndices);

	_uint index = 0;
	for (_uint i = 0; i < pMesh->mNumFaces; ++i)
	{
		const aiFace& face = pMesh->mFaces[i];

		m_Indices[index++] = face.mIndices[0];
		m_Indices[index++] = face.mIndices[1];
		m_Indices[index++] = face.mIndices[2];
	}

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = m_Indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

HRESULT FBXMesh::ExportMeshFormat(MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices)
{
	meshFormat.materialIndex = m_iMaterialIndex;
	meshFormat.numVertices = m_iNumVertices;
	meshFormat.vertexStride = m_iVertexStride;
	meshFormat.numIndices = m_iNumIndices;
	meshFormat.indexStride = m_iIndexStride;

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		VTX_FORMAT vertex{};

		vertex.position = m_Vertices[i].position;
		vertex.normal = m_Vertices[i].normal;
		vertex.texCoord = m_Vertices[i].texCoord;
		vertex.tangent = m_Vertices[i].tangent;

		vertices.push_back(vertex);
	}

	for (_uint i = 0; i < m_iNumIndices; ++i)
		indices.push_back(m_Indices[i]);

	return S_OK;
}

void FBXMesh::Free()
{
	__super::Free();
}

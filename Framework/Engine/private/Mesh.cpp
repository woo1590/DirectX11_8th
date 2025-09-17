#include "EnginePCH.h"
#include "Mesh.h"
#include "Bone.h"

Mesh::Mesh()
	:VIBuffer()
{
}

Mesh* Mesh::Create(ModelType eType, const MESH_FORMAT& mesh, std::vector<VTX_FORMAT>& vertices, std::vector<_uint>& indices, _fmatrix preTransformMatrix)
{
	Mesh* Instance = new Mesh();

	if (FAILED(Instance->Initialize(eType, mesh, vertices, indices, preTransformMatrix)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Mesh::Initialize(ModelType eType, const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats, _fmatrix preTransformMatrix)
{
	if (FAILED(eType == ModelType::Static ? CreateStaticMesh(meshFormat, vertexFormats, indexFormats, preTransformMatrix) :
											CreateSkinnedMesh(meshFormat, vertexFormats, indexFormats)))
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

void Mesh::ExtractBoneMatrices(RenderProxy& proxy, std::vector<Bone*>& bones)
{
	proxy.numBones = m_iNumBones;
	for (_uint i = 0; i < m_iNumBones; ++i)
		XMStoreFloat4x4(&m_BoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * bones[m_BoneIndices[i]]->GetCombinedTransformationMatrix());
	proxy.boneMatrices = m_BoneMatrices.data();
}

void Mesh::Free()
{
	__super::Free();
}

HRESULT Mesh::CreateStaticMesh(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats, _fmatrix preTransformMatrix)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = meshFormat.numVertices;
	m_iVertexStride = sizeof(VTXMESH);

	m_iNumIndices = meshFormat.numIndices;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_strName = meshFormat.name;
	m_iMaterialIndex = meshFormat.materialIndex;

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
		XMVector3TransformCoord(XMLoadFloat3(&vertices[i].position), preTransformMatrix);

		vertices[i].normal = vertexFormats[i].normal;
		XMVector3TransformNormal(XMLoadFloat3(&vertices[i].normal), preTransformMatrix);

		vertices[i].texCoord = vertexFormats[i].texCoord;
		vertices[i].tangent = vertexFormats[i].tangent;
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT Mesh::CreateSkinnedMesh(const MESH_FORMAT& meshFormat, std::vector<VTX_FORMAT>& vertexFormats, std::vector<_uint>& indexFormats)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = meshFormat.numVertices;
	m_iVertexStride = sizeof(VTXSKINNEDMESH);

	m_iNumIndices = meshFormat.numIndices;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_strName = meshFormat.name;
	m_iMaterialIndex = meshFormat.materialIndex;
	m_iNumBones = meshFormat.numBones;
	m_BoneMatrices.resize(m_iNumBones);

	m_OffsetMatrices.resize(m_iNumBones);
	memcpy_s(m_OffsetMatrices.data(), sizeof(_float4x4) * m_iNumBones, meshFormat.offsetMatrices, sizeof(_float4x4) * m_iNumBones);
	m_BoneIndices.resize(m_iNumBones);
	memcpy_s(m_BoneIndices.data(), sizeof(_uint) * m_iNumBones, meshFormat.boneIndices, sizeof(_uint) * m_iNumBones);

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXSKINNEDMESH> vertices(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		vertices[i].position = vertexFormats[i].position;
		vertices[i].normal = vertexFormats[i].normal;
		vertices[i].texCoord = vertexFormats[i].texCoord;
		vertices[i].tangent = vertexFormats[i].tangent;
		vertices[i].blendIndex = vertexFormats[i].blendIndex;
		vertices[i].blendWeight = vertexFormats[i].blendWeight;
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

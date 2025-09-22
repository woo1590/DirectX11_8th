#include "EnginePCH.h"
#include "Mesh.h"
#include "Bone.h"

Mesh::Mesh()
	:VIBuffer()
{
}

Mesh* Mesh::Create(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix)
{
	Mesh* Instance = new Mesh();

	if (FAILED(Instance->Initialize(eType, file, preTransformMatrix)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Mesh::Initialize(ModelType eType, std::ifstream& file, _fmatrix preTransformMatrix)
{
	MESH_FORMAT meshFormat{};
	std::vector<VTX_FORMAT> vertexFormats{};
	std::vector<_uint> indexFormats{};

	if (FAILED(eType == ModelType::Static ? CreateStaticMesh(file, preTransformMatrix) :
											CreateSkinnedMesh(file)))
		return E_FAIL;

	return S_OK;
}

void Mesh::ComputeBonePalette(const std::vector<_float4x4>& combinedMatices, std::vector<_float4x4>& bonePalette)
{
	if (!bonePalette.size())
		bonePalette.resize(m_iNumBones);

	for (_uint i = 0; i < m_iNumBones; ++i)
		XMStoreFloat4x4(&bonePalette[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(&combinedMatices[m_BoneIndices[i]]));

}

void Mesh::Free()
{
	__super::Free();
}

HRESULT Mesh::CreateStaticMesh(std::ifstream& file, _fmatrix preTransformMatrix)
{
	MESH_FORMAT meshFormat{};
	file.read(reinterpret_cast<char*>(&meshFormat), sizeof(MESH_FORMAT));

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
	std::vector<VTX_FORMAT> vertexFormats(m_iNumVertices);
	file.read(reinterpret_cast<char*>(vertexFormats.data()), sizeof(VTX_FORMAT) * m_iNumVertices);

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

	/*----Index Buffer----*/
	std::vector<_uint> indexFormats(m_iNumIndices);
	file.read(reinterpret_cast<char*>(indexFormats.data()), sizeof(_uint) * m_iNumIndices);

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

HRESULT Mesh::CreateSkinnedMesh(std::ifstream& file)
{
	MESH_FORMAT meshFormat{};
	file.read(reinterpret_cast<char*>(&meshFormat), sizeof(MESH_FORMAT));

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

	m_BoneIndices.resize(m_iNumBones);
	memcpy_s(m_BoneIndices.data(), sizeof(_uint) * m_iNumBones, meshFormat.boneIndices, sizeof(_uint) * m_iNumBones);

	/*Offset Matrices*/
	m_OffsetMatrices.resize(m_iNumBones);
	file.read(reinterpret_cast<char*>(m_OffsetMatrices.data()), sizeof(_float4x4) * m_iNumBones);

	/*----Vertex Buffer----*/
	std::vector<VTX_FORMAT> vertexFormats(m_iNumVertices);
	file.read(reinterpret_cast<char*>(vertexFormats.data()), sizeof(VTX_FORMAT) * m_iNumVertices);

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

	/*----Index Buffer----*/
	std::vector<_uint> indexFormats(m_iNumIndices);
	file.read(reinterpret_cast<char*>(indexFormats.data()), sizeof(_uint) * m_iNumIndices);

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

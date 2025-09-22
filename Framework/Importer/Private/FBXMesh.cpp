#include "ImporterPCH.h"
#include "FBXMesh.h"
#include "FBXLoaderComponent.h"
#include "FBXSkeleton.h"
#include "FBXBone.h"

FBXMesh::FBXMesh()
	:VIBuffer()
{
}

FBXMesh * FBXMesh::Create(aiMesh* pMesh, ModelType eType, FBXSkeleton* pSkeleton)
{
	FBXMesh* Instance = new FBXMesh();

	if (FAILED(Instance->Initialize(pMesh, eType, pSkeleton)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT FBXMesh::Initialize(aiMesh* pMesh, ModelType eType, FBXSkeleton* pSkeleton)
{
	m_strName = pMesh->mName.C_Str();

	/*----Vertex Buffer----*/
	if (FAILED(eType == ModelType::Static ? CreateStaticMesh(pMesh) : CreateSkinnedMesh(pMesh, pSkeleton)))
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

void FBXMesh::ExtractBoneMatrices(RenderProxy& proxy, FBXSkeleton * pSkeleton)
{
	proxy.numBones = m_iNumBones;
	for (_uint i = 0; i < m_iNumBones; ++i)
		XMStoreFloat4x4(&m_BoneMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * pSkeleton->GetCombinedTransformMatrix(m_BoneIndices[i]));

	proxy.boneMatrices = m_BoneMatrices.data();
}

HRESULT FBXMesh::Export(std::ofstream& out)
{
	MESH_FORMAT meshFormat{};
	strncpy_s(meshFormat.name, MAX_PATH, m_strName.c_str(), sizeof(meshFormat.name) - 1);
	meshFormat.materialIndex = m_iMaterialIndex;
	meshFormat.numVertices = m_iNumVertices;
	meshFormat.vertexStride = m_iVertexStride;
	meshFormat.numIndices = m_iNumIndices;
	meshFormat.indexStride = m_iIndexStride;
	meshFormat.numBones = m_iNumBones;
	memcpy_s(&meshFormat.boneIndices, sizeof(_uint) * MAX_BONES, m_BoneIndices.data(), sizeof(_uint) * m_iNumBones);

	out.write(reinterpret_cast<const char*>(&meshFormat), sizeof(MESH_FORMAT));
	out.write(reinterpret_cast<const char*>(m_OffsetMatrices.data()), sizeof(_float4x4) * m_iNumBones);

	/*Vertex Format*/
	std::vector<VTX_FORMAT> vertices;
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		VTX_FORMAT vertex{};

		vertex.position = m_VertexFormats[i].position;
		vertex.normal = m_VertexFormats[i].normal;
		vertex.texCoord = m_VertexFormats[i].texCoord;
		vertex.tangent = m_VertexFormats[i].tangent;
		vertex.blendIndex = m_VertexFormats[i].blendIndex;
		vertex.blendWeight = m_VertexFormats[i].blendWeight;

		vertices.push_back(vertex);
	}
	out.write(reinterpret_cast<const char*>(vertices.data()), sizeof(VTX_FORMAT) * m_iNumVertices);

	/*Index Format*/
	std::vector<_uint> indices;
	for (_uint i = 0; i < m_iNumIndices; ++i)
		indices.push_back(m_Indices[i]);

	out.write(reinterpret_cast<const char*>(indices.data()), sizeof(_uint) * m_iNumIndices);
	
	return S_OK;
}

void FBXMesh::Free()
{
	__super::Free();
}

HRESULT FBXMesh::CreateStaticMesh(aiMesh* pMesh)
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

	std::vector<VTXMESH> vertices(m_iNumVertices);
	m_VertexFormats.resize(m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy_s(&vertices[i].position, sizeof(_float3), &pMesh->mVertices[i], sizeof(_float3));
		memcpy_s(&vertices[i].normal, sizeof(_float3), &pMesh->mNormals[i], sizeof(_float3));
		memcpy_s(&vertices[i].texCoord, sizeof(_float2), &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy_s(&vertices[i].tangent, sizeof(_float3), &pMesh->mTangents[i], sizeof(_float3));

		/*----Vertex Format----*/
		memcpy_s(&m_VertexFormats[i].position, sizeof(_float3), &pMesh->mVertices[i], sizeof(_float3));
		memcpy_s(&m_VertexFormats[i].normal, sizeof(_float3), &pMesh->mNormals[i], sizeof(_float3));
		memcpy_s(&m_VertexFormats[i].texCoord, sizeof(_float2), &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy_s(&m_VertexFormats[i].tangent, sizeof(_float3), &pMesh->mTangents[i], sizeof(_float3));
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT FBXMesh::CreateSkinnedMesh(aiMesh* pMesh, FBXSkeleton* pSkeleton)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pMesh->mNumVertices;
	m_iVertexStride = sizeof(VTXSKINNEDMESH);

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

	std::vector<VTXSKINNEDMESH> vertices(m_iNumVertices);
	m_VertexFormats.resize(m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy_s(&vertices[i].position, sizeof(_float3), &pMesh->mVertices[i], sizeof(_float3));
		memcpy_s(&vertices[i].normal, sizeof(_float3), &pMesh->mNormals[i], sizeof(_float3));
		memcpy_s(&vertices[i].texCoord, sizeof(_float2), &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy_s(&vertices[i].tangent, sizeof(_float3), &pMesh->mTangents[i], sizeof(_float3));

		/*----Vertex Format----*/
		memcpy_s(&m_VertexFormats[i].position, sizeof(_float3), &pMesh->mVertices[i], sizeof(_float3));
		memcpy_s(&m_VertexFormats[i].normal, sizeof(_float3), &pMesh->mNormals[i], sizeof(_float3));
		memcpy_s(&m_VertexFormats[i].texCoord, sizeof(_float2), &pMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy_s(&m_VertexFormats[i].tangent, sizeof(_float3), &pMesh->mTangents[i], sizeof(_float3));

	}

	m_iNumBones = pMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)				//영향을 주는 뼈의 갯수
	{
		aiBone* bone = pMesh->mBones[i];				//i번째 뼈
		_int boneIndex = pSkeleton->GetBoneIndexByName(bone->mName.data);
		if (boneIndex == -1)
		{
			MSG_BOX("Missing Bone");					//일치하는 뼈가 없음, 이 모델 못씀
			return E_FAIL;
		}
		_float4x4 offsetMatrix{};
		memcpy_s(&offsetMatrix, sizeof(_float4x4), &bone->mOffsetMatrix, sizeof(_float4x4));
		XMStoreFloat4x4(&offsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&offsetMatrix)));

		m_BoneIndices.push_back(boneIndex);
		m_OffsetMatrices.push_back(offsetMatrix);

		_uint numWeights = bone->mNumWeights;			//i번째 뼈가 영향을 주는 정점의 갯수

		for (_uint j = 0; j < numWeights; ++j)
		{
			aiVertexWeight weight = bone->mWeights[j];	//i번째 뼈가 영향을 주는 j번째 정점에 대한 정보

			_uint vertexIndex = weight.mVertexId;		//j번째 정점의 실제 인덱스
			_float vertexWeight = weight.mWeight;		//j번째 정점에 대한 가중치
			if (0.f == vertices[vertexIndex].blendWeight.x)
			{
				vertices[vertexIndex].blendIndex.x = i;
				vertices[vertexIndex].blendWeight.x = vertexWeight;

				m_VertexFormats[vertexIndex].blendIndex.x = i;
				m_VertexFormats[vertexIndex].blendWeight.x = vertexWeight;
			}
			else if (0.f == vertices[vertexIndex].blendWeight.y)
			{
				vertices[vertexIndex].blendIndex.y = i;
				vertices[vertexIndex].blendWeight.y = vertexWeight;

				m_VertexFormats[vertexIndex].blendIndex.y = i;
				m_VertexFormats[vertexIndex].blendWeight.y = vertexWeight;
			}
			else if (0.f == vertices[vertexIndex].blendWeight.z)
			{
				vertices[vertexIndex].blendIndex.z = i;
				vertices[vertexIndex].blendWeight.z = vertexWeight;

				m_VertexFormats[vertexIndex].blendIndex.z = i;
				m_VertexFormats[vertexIndex].blendWeight.z = vertexWeight;
			}
			else if (0.f == vertices[vertexIndex].blendWeight.w)
			{
				_float4 weight = vertices[i].blendWeight;

				vertices[vertexIndex].blendIndex.w = i;
				vertices[vertexIndex].blendWeight.w = 1.f - (weight.x + weight.y + weight.z);

				m_VertexFormats[vertexIndex].blendIndex.w = i;
				m_VertexFormats[vertexIndex].blendWeight.w = 1.f - (weight.x + weight.y + weight.z);
			}
		}
	}

	if (!m_iNumBones)
	{
		m_iNumBones = 1;
		_int boneIndex = pSkeleton->GetBoneIndexByName(pMesh->mName.data);
		if (boneIndex == -1)
		{
			MSG_BOX("Missing Bone!");					//일치하는 뼈가 없음 -> 말도 안되는상황
			return E_FAIL;
		}
		_float4x4 offsetMatrix{};
		XMStoreFloat4x4(&offsetMatrix, XMMatrixIdentity());

		m_BoneIndices.push_back(boneIndex);
		m_OffsetMatrices.push_back(offsetMatrix);
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	m_BoneMatrices.resize(m_iNumBones);

	return S_OK;
}

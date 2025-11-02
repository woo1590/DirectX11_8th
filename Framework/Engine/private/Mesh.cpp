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

	ComputeBoundingBox(eType);

	return S_OK;
}

void Mesh::ComputeBonePalette(const std::vector<_float4x4>& combinedMatices, std::vector<_float4x4>& bonePalette)
{
	if (!bonePalette.size())
		bonePalette.resize(m_iNumBones);

	for (_uint i = 0; i < m_iNumBones; ++i)
		XMStoreFloat4x4(&bonePalette[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(&combinedMatices[m_BoneIndices[i]]));

}

RAY_HIT_DATA Mesh::RayCast(RAY localRay, PickingType type)
{
	RAY_HIT_DATA hitData{};

	_float3 normal{};
	_bool isHit = false;
	_float tMesh = FLT_MAX;
	_float tHit = FLT_MAX;

	if (m_IsStaticMesh)
	{
		RayCastBVHNode(0, localRay, hitData);
	}
	else
	{
		if (m_BoundingBox.Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction), tMesh))
		{
			if (type == PickingType::BoundingBox)
			{
				hitData.isHit = true;
				hitData.localDistance = tMesh;
			}
			else
			{
				for (_uint i = 0; i < m_iNumIndices / 3; ++i)
				{
					_uint index = i * 3;

					_float3 p0 = m_VertexPositions[m_Indices[index]];
					_float3 p1 = m_VertexPositions[m_Indices[index + 1]];
					_float3 p2 = m_VertexPositions[m_Indices[index + 2]];

					_float tTri{};
					if (TriangleTests::Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction),
						XMLoadFloat3(&p0), XMLoadFloat3(&p1), XMLoadFloat3(&p2), tTri))
					{
						if (tTri > 0.f && tTri < tHit)
						{
							tHit = tTri;

							_vector ab = XMLoadFloat3(&p1) - XMLoadFloat3(&p0);
							_vector ac = XMLoadFloat3(&p2) - XMLoadFloat3(&p0);
							XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(ab, ac)));
						}
					}
				}
			}
		}
	}

	if (tHit < FLT_MAX)
	{
		hitData.isHit = true;
		hitData.localDistance = tHit;
		hitData.normal = normal;
	}
	return hitData;
}

BOUNDING_BOX_DATA Mesh::GetMeshBoundingBoxData() const
{
	BOUNDING_BOX_DATA data{};
	data.AABBMin = m_AABBMin;
	data.AABBMax = m_AABBMax;

	return data;
}

void Mesh::Free()
{
	__super::Free();
}

HRESULT Mesh::CreateStaticMesh(std::ifstream& file, _fmatrix preTransformMatrix)
{
	m_IsStaticMesh = true;

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
	m_VertexPositions.resize(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		vertices[i].position = vertexFormats[i].position;
		XMStoreFloat3(&vertices[i].position, XMVector3TransformCoord(XMLoadFloat3(&vertices[i].position), preTransformMatrix));

		vertices[i].normal = vertexFormats[i].normal;
		XMStoreFloat3(&vertices[i].normal, XMVector3TransformNormal(XMLoadFloat3(&vertices[i].normal), preTransformMatrix));

		vertices[i].texCoord = vertexFormats[i].texCoord;
		vertices[i].tangent = vertexFormats[i].tangent;

		/*For BoundingBox*/
		m_VertexPositions[i] = vertices[i].position;
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

	/*For BoundingBox*/
	m_Indices.resize(m_iNumIndices);
	memcpy_s(m_Indices.data(), sizeof(_uint) * m_iNumIndices, indexFormats.data(), sizeof(_uint) * m_iNumIndices);

	return S_OK;
}

HRESULT Mesh::CreateSkinnedMesh(std::ifstream& file)
{
	m_IsStaticMesh = false;

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
	m_VertexPositions.resize(m_iNumVertices);
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		vertices[i].position = vertexFormats[i].position;
		vertices[i].normal = vertexFormats[i].normal;
		vertices[i].texCoord = vertexFormats[i].texCoord;
		vertices[i].tangent = vertexFormats[i].tangent;
		vertices[i].blendIndex = vertexFormats[i].blendIndex;
		vertices[i].blendWeight = vertexFormats[i].blendWeight;

		/*For BoundingBox*/
		m_VertexPositions[i] = vertices[i].position;
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

	/*For BoundingBox*/
	m_Indices.resize(m_iNumIndices);
	memcpy_s(m_Indices.data(), sizeof(_uint) * m_iNumIndices, indexFormats.data(), sizeof(_uint) * m_iNumIndices);

	return S_OK;
}

void Mesh::ComputeBoundingBox(ModelType eType)
{
	_float3 min{ FLT_MAX,FLT_MAX,FLT_MAX };
	_float3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_float3 position = m_VertexPositions[i];

		if (position.x < min.x) min.x = position.x;
		if (position.y < min.y)min.y = position.y;
		if (position.z < min.z)min.z = position.z;

		if (position.x > max.x) max.x = position.x;
		if (position.y > max.y)max.y = position.y;
		if (position.z > max.z)max.z = position.z;
	}
	m_AABBMin = min;
	m_AABBMax = max;

	m_BoundingBox.CreateFromPoints(m_BoundingBox, XMLoadFloat3(&m_AABBMin), XMLoadFloat3(&m_AABBMax));
	if (!m_IsStaticMesh)
		return;

	/*build cluster node  ----- only static mesh*/
	_uint numTriangle = m_iNumIndices / 3;
	for (_uint i = 0; i < numTriangle; ++i)
	{
		_uint index = i * 3;
		TRIANGLE_DESC triangle{};
		triangle.position[0] = m_VertexPositions[m_Indices[index]];
		triangle.position[1] = m_VertexPositions[m_Indices[index + 1]];
		triangle.position[2] = m_VertexPositions[m_Indices[index + 2]];
		triangle.index[0] = m_Indices[index];
		triangle.index[1] = m_Indices[index + 1];
		triangle.index[2] = m_Indices[index + 2];
		
		_float3 aabbMin
		{
			(std::min)((std::min)(triangle.position[0].x, triangle.position[1].x),triangle.position[2].x),
			(std::min)((std::min)(triangle.position[0].y, triangle.position[1].y),triangle.position[2].y),
			(std::min)((std::min)(triangle.position[0].z, triangle.position[1].z),triangle.position[2].z),
		};

		_float3 aabbMax
		{
			(std::max)((std::max)(triangle.position[0].x, triangle.position[1].x),triangle.position[2].x),
			(std::max)((std::max)(triangle.position[0].y, triangle.position[1].y),triangle.position[2].y),
			(std::max)((std::max)(triangle.position[0].z, triangle.position[1].z),triangle.position[2].z),
		};
		triangle.triAABB.CreateFromPoints(triangle.triAABB, XMLoadFloat3(&aabbMin), XMLoadFloat3(&aabbMax));

		m_Triangles.push_back(triangle);
	}

	std::vector<_uint> triIndices(numTriangle);
	for (_uint i = 0; i < numTriangle; ++i)
		triIndices[i] = i;

	BuildBVHNode(triIndices, 0, numTriangle);
	m_BVHTriIndices = std::move(triIndices);
}

_uint Mesh::BuildBVHNode(std::vector<_uint>& triIndices, _uint startIndex, _uint endIndex)
{
	BVH_NODE node{};
	_uint axis{};
	_uint nodeIndex = m_BVHNodes.size();
	_uint count = endIndex - startIndex;

	_float3 nodeAABBMin{FLT_MAX,FLT_MAX,FLT_MAX};
	_float3 nodeAABBMax{-FLT_MAX,-FLT_MAX,-FLT_MAX};
	for (_uint i = startIndex; i < endIndex; ++i)
	{
		BoundingBox triAABB = m_Triangles[triIndices[i]].triAABB;
		nodeAABBMin.x = (std::min)(triAABB.Center.x - triAABB.Extents.x, nodeAABBMin.x);
		nodeAABBMin.y = (std::min)(triAABB.Center.y - triAABB.Extents.y, nodeAABBMin.y);
		nodeAABBMin.z = (std::min)(triAABB.Center.z - triAABB.Extents.z, nodeAABBMin.z);

		nodeAABBMax.x = (std::max)(triAABB.Center.x + triAABB.Extents.x, nodeAABBMax.x);
		nodeAABBMax.y = (std::max)(triAABB.Center.y + triAABB.Extents.y, nodeAABBMax.y);
		nodeAABBMax.z = (std::max)(triAABB.Center.z + triAABB.Extents.z, nodeAABBMax.z);
	}
	node.nodeAABB.CreateFromPoints(node.nodeAABB, XMLoadFloat3(&nodeAABBMin), XMLoadFloat3(&nodeAABBMax));
	m_BVHNodes.push_back(node);

	/*make leaf*/
	if (count < m_iLeafCount)
	{
		m_BVHNodes[nodeIndex].firstIndex = startIndex;
		m_BVHNodes[nodeIndex].count = count;
		m_BVHNodes[nodeIndex].left = -1;
		m_BVHNodes[nodeIndex].right = -1;
		return nodeIndex;
	}

	if (node.nodeAABB.Extents.x > node.nodeAABB.Extents.y && node.nodeAABB.Extents.x > node.nodeAABB.Extents.z)
		axis = 0;
	else if (node.nodeAABB.Extents.y > node.nodeAABB.Extents.x && node.nodeAABB.Extents.y > node.nodeAABB.Extents.z)
		axis = 1;
	else
		axis = 2;	

	if (node.nodeAABB.Extents.x < math::ELIPSON && node.nodeAABB.Extents.y < math::ELIPSON && node.nodeAABB.Center.z < math::ELIPSON)
	{
		m_BVHNodes[nodeIndex].firstIndex = startIndex;
		m_BVHNodes[nodeIndex].count = count;
		m_BVHNodes[nodeIndex].left = -1;
		m_BVHNodes[nodeIndex].right = -1;
		return nodeIndex;
	}

	_uint mid = startIndex + count / 2;
	if (mid == startIndex || mid == endIndex)
	{
		m_BVHNodes[nodeIndex].firstIndex = startIndex;
		m_BVHNodes[nodeIndex].count = count;
		m_BVHNodes[nodeIndex].left = -1;
		m_BVHNodes[nodeIndex].right = -1;
		return nodeIndex;
	}

	auto iterStart = triIndices.begin() + startIndex;
	auto iterMid = triIndices.begin() + mid;
	auto iterEnd = triIndices.begin() + endIndex;

	std::nth_element(iterStart, iterMid, iterEnd, [&](_uint a, _uint b)
		{
			if (0 == axis)
				return m_Triangles[a].triAABB.Center.x < m_Triangles[b].triAABB.Center.x;
			else if (1 == axis)
				return m_Triangles[a].triAABB.Center.y < m_Triangles[b].triAABB.Center.y;
			else
				return m_Triangles[a].triAABB.Center.z < m_Triangles[b].triAABB.Center.z;
		});

	_uint left = BuildBVHNode(triIndices, startIndex, mid);
	_uint right = BuildBVHNode(triIndices, mid, endIndex);
	m_BVHNodes[nodeIndex].count = 0;
	m_BVHNodes[nodeIndex].left = left;
	m_BVHNodes[nodeIndex].right = right;

	return nodeIndex;
}

void Mesh::RayCastBVHNode(_uint nodeIndex, RAY localRay, RAY_HIT_DATA& out)
{
	auto& node = m_BVHNodes[nodeIndex];

	RAY_HIT_DATA result{};

	_float distance{};
	_float3 normal{};
	if (node.nodeAABB.Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction), distance))
	{
		/*is leaf*/
		if (node.count)
		{
			_float tHit = FLT_MAX;
			for (_uint i = 0; i < node.count; ++i)
			{
				_float tTri{};
				TRIANGLE_DESC triangle = m_Triangles[m_BVHTriIndices[node.firstIndex + i]];

				if (TriangleTests::Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction),
					XMLoadFloat3(&triangle.position[0]), XMLoadFloat3(&triangle.position[1]), XMLoadFloat3(&triangle.position[2]), tTri))
				{
					if (tTri > 0.f && tTri < tHit)
					{
						tHit = tTri;

						_vector ab = XMLoadFloat3(&triangle.position[1]) - XMLoadFloat3(&triangle.position[0]);
						_vector ac = XMLoadFloat3(&triangle.position[2]) - XMLoadFloat3(&triangle.position[0]);
						XMStoreFloat3(&normal, XMVector3Normalize(XMVector3Cross(ab, ac)));
					}
				}
			}

			if (tHit < out.localDistance)
			{
				out.isHit = true;
				out.localDistance = tHit;
				out.normal = normal;
			}
		}
		else
		{
			_float leftDistance = FLT_MAX;
			_float rightDistance = FLT_MAX;
			_bool leftResult = false;
			_bool rightResult = false;

			leftResult = m_BVHNodes[node.left].nodeAABB.Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction), leftDistance);
			rightResult = m_BVHNodes[node.right].nodeAABB.Intersects(XMLoadFloat3(&localRay.origin), XMLoadFloat3(&localRay.direction), rightDistance);

			if (leftResult && rightResult)
			{
				if (leftDistance < rightDistance)
				{
					RayCastBVHNode(node.left, localRay, out);
					if (rightDistance < out.localDistance)
						RayCastBVHNode(node.right, localRay, out);
				}
				else
				{
					RayCastBVHNode(node.right, localRay, out);
					if (leftDistance < out.localDistance)
						RayCastBVHNode(node.left, localRay, out);
				}
			}
			else if (leftResult)
				RayCastBVHNode(node.left, localRay, out);
			else
				RayCastBVHNode(node.right, localRay, out);
		}
	}
	else
		return;
}

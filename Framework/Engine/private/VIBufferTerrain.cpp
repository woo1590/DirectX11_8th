#include "EnginePCH.h"
#include "VIBufferTerrain.h"

VIBufferTerrain::VIBufferTerrain()
	:VIBuffer()
{
}

VIBufferTerrain* VIBufferTerrain::Create(const _string& filePath)
{
	VIBufferTerrain* Instance = new VIBufferTerrain();

	if (FAILED(Instance->Initialize(filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBufferTerrain::Initialize(const _string& filePath)
{
	_ulong dwByte{};
	HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (!hFile)
		return E_FAIL;

	BITMAPFILEHEADER fh{};
	if (!ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, 0))
		return E_FAIL;

	BITMAPINFOHEADER ih{};
	if (!ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, 0))
		return E_FAIL;

	std::vector<_uint> pixels(ih.biWidth * ih.biHeight);
	if (!ReadFile(hFile, pixels.data(), sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, 0))
		return E_FAIL;

	if (!CloseHandle(hFile))
		return E_FAIL;

	m_iNumVertexBuffers = 1;
	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iVertexStride = sizeof(VTXNORTEX);

	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_iIndexStride = 4;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*----Vertex Buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	std::vector<VTXNORTEX> vertices(m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint index = i * m_iNumVerticesX + j;

			vertices[index].position = _float3(j, (pixels[index] & 0x000000ff) / 10.f, i);
			vertices[index].normal = _float3(0.f, 0.f, 0.f);
			vertices[index].texCoord = _float2(j / (m_iNumVerticesX - 1.f), 1.f - (i / (m_iNumVerticesZ - 1.f)));
		}
	}

	D3D11_SUBRESOURCE_DATA vbInitData{};
	vbInitData.pSysMem = vertices.data();
	vbInitData.SysMemPitch = 0;
	vbInitData.SysMemSlicePitch = 0;

	/*----Index Buffer----*/
	D3D11_BUFFER_DESC ibDesc{};
	ibDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = m_iIndexStride;

	std::vector<_uint> indices(m_iNumIndices);
	_uint numIndex = 0;
	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint index = i * m_iNumVerticesX + j;

			indices[numIndex++] = index + m_iNumVerticesX;
			indices[numIndex++] = index + m_iNumVerticesX + 1;
			indices[numIndex++] = index + 1;

			indices[numIndex++] = index + m_iNumVerticesX;
			indices[numIndex++] = index + 1;
			indices[numIndex++] = index;
		}
	}

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;

	/*----Compute Vertex Normal----*/
	for (_uint i = 0; i < m_iNumIndices / 3; ++i)
	{
		_uint i0 = indices[i * 3];
		_uint i1 = indices[i * 3 + 1];
		_uint i2 = indices[i * 3 + 2];

		_vector p0 = XMLoadFloat3(&vertices[i0].position);
		_vector p1 = XMLoadFloat3(&vertices[i1].position);
		_vector p2 = XMLoadFloat3(&vertices[i2].position);
		
		_vector u = p1 - p0;
		_vector v = p2 - p1;
		_vector normal = XMVector3Normalize(XMVector3Cross(u, v));

		XMStoreFloat3(&vertices[i0].normal, XMLoadFloat3(&vertices[i0].normal) + normal);
		XMStoreFloat3(&vertices[i1].normal, XMLoadFloat3(&vertices[i1].normal) + normal);
		XMStoreFloat3(&vertices[i2].normal, XMLoadFloat3(&vertices[i2].normal) + normal);
	}

	for (auto& vertex : vertices)
		XMStoreFloat3(&vertex.normal, XMVector3Normalize(XMLoadFloat3(&vertex.normal)));

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, &vbInitData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

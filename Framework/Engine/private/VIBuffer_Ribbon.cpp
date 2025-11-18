#include "EnginePCH.h"
#include "VIBuffer_Ribbon.h"

VIBuffer_Ribbon::VIBuffer_Ribbon()
	:VIBuffer()
{
}

VIBuffer_Ribbon* VIBuffer_Ribbon::Create(_uint numMaxPoints)
{
	VIBuffer_Ribbon* Instance = new VIBuffer_Ribbon();

	if (FAILED(Instance->Initialize(numMaxPoints)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT VIBuffer_Ribbon::Initialize(_uint numMaxPoints)
{
	m_iNumVertexBuffers = 1;

	m_iVertexStride = sizeof(VTX_RIBBON);
	m_iNumVertices = numMaxPoints;

	m_iIndexStride = 2;
	m_iNumIndices = (numMaxPoints - 1) * 2;

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	/*----Vertex buffer----*/
	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = m_iVertexStride;

	if (FAILED(m_pDevice->CreateBuffer(&vbDesc, nullptr, &m_pVB)))
		return E_FAIL;

	/*----Index buffer----*/
	D3D11_BUFFER_DESC ibDesc{};
	ibDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = m_iIndexStride;

	std::vector<_ushort> indices;
	indices.reserve(m_iNumIndices);
	for (_uint i = 0; i < m_iNumIndices - 1; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	D3D11_SUBRESOURCE_DATA ibInitData{};
	ibInitData.pSysMem = indices.data();
	ibInitData.SysMemPitch = 0;
	ibInitData.SysMemSlicePitch = 0;

	if (FAILED(m_pDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void VIBuffer_Ribbon::Free()
{
	__super::Free();
}

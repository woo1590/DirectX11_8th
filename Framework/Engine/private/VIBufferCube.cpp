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
	

	return S_OK;
}

void VIBufferCube::Free()
{
	__super::Free();
}

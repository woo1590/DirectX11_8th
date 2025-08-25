#include "EnginePCH.h"
#include "CubeMesh.h"

CubeMesh::CubeMesh()
	:Mesh()
{
}

CubeMesh* CubeMesh::Create()
{
	CubeMesh* Instance = new CubeMesh();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT CubeMesh::Initialize()
{
	m_iVertexCnt = 8;
	m_iIndexCnt = 36;

	D3D11_BUFFER_DESC vbDESC{};

	return S_OK;
}

void CubeMesh::Free()
{
	__super::Free();
}

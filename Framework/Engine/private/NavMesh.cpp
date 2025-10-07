#include "EnginePCH.h"
#include "NavMesh.h"
#include "Cell.h"

NavMesh::NavMesh()
{
}

NavMesh* NavMesh::Create(const _string& filePath)
{
	NavMesh* Instance = new NavMesh();

	if (FAILED(Instance->Initialize(filePath)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavMesh::Initialize(const _string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		MSG_BOX("Failed to open : NavMesh file");
		return E_FAIL;
	}

	file.read(reinterpret_cast<char*>(&m_iNumCells), sizeof(_uint));

	std::vector<CELL_DESC> cellDatas;
	cellDatas.resize(m_iNumCells);
	file.read(reinterpret_cast<char*>(cellDatas.data()), sizeof(CELL_DESC) * m_iNumCells);

	for (_uint i = 0; i < m_iNumCells; ++i)
	{
		Cell* cell = Cell::Create(cellDatas[i]);
		if (!cell)
		{
			MSG_BOX("Failed to create : Cell");
			return E_FAIL;
		}
		m_Cells.push_back(cell);
	}

	return S_OK;
}

void NavMesh::Free()
{
	__super::Free();

	for (auto& cell : m_Cells)
		Safe_Release(cell);
	m_Cells.clear();
}

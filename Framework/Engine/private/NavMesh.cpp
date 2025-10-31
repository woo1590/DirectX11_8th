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

	std::vector<NAVCELL_DATA> cellDatas;
	cellDatas.resize(m_iNumCells);
	file.read(reinterpret_cast<char*>(cellDatas.data()), sizeof(NAVCELL_DATA) * m_iNumCells);

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

HRESULT NavMesh::ExtractDebugProxies(std::vector<RenderProxy>& proxies)
{
	for (const auto& cell : m_Cells)
	{
		RenderProxy proxy{};
		
		XMStoreFloat4x4(&proxy.worldMatrix, XMMatrixIdentity());
		proxy.buffer = cell->GetBuffer();
		proxy.materialInstance = cell->GetMaterialInstance();

		proxies.push_back(proxy);
	}

	return S_OK;
}

_float3 NavMesh::GetCellNormal(_uint cellIndex)
{
	return m_Cells[cellIndex]->GetCellNormal();
}

_bool NavMesh::IsCellExist(_uint cellIndex)
{
	if (cellIndex >= m_Cells.size())
		return false;

	return true;
}

_float3 NavMesh::GetPositionInCell(_uint cellIndex)
{
	return m_Cells[cellIndex]->GetPositionInCell();
}

_bool NavMesh::IsLinkedCell(_float3 position, _uint& currCellIndex)
{
	return m_Cells[currCellIndex]->IsLinked(position, currCellIndex);
}

_bool NavMesh::IsMove(_float3 position, _uint& currCellIndex)
{
	_int neighborIndex = -1;
	std::unordered_set<_uint> visited;
	visited.emplace(currCellIndex);
	
	if (!m_Cells[currCellIndex]->IsInCell(position, neighborIndex))
	{
		if (-1 != neighborIndex)
		{
			while (true)
			{
				visited.emplace(neighborIndex);

				if (m_Cells[neighborIndex]->IsInCell(position, neighborIndex))
				{
					int a = 1;
					break;
				}

				if (visited.count(neighborIndex))
				{
					int a = 1;
					break;
				}

				if (-1 == neighborIndex)
					return false;
			}

			currCellIndex = neighborIndex;
			return true;
		}
		else
			return false;
	}
	else
		return true;
}

_float NavMesh::GetHeight(_float3 position, _uint currCellIndex)
{
	return m_Cells[currCellIndex]->GetHeight(position);
}

_float3 NavMesh::MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellIndex)
{
	return m_Cells[currCellIndex]->MakeSlideVector(position, nextPosition, currCellIndex);
}

void NavMesh::Free()
{
	__super::Free();

	for (auto& cell : m_Cells)
		Safe_Release(cell);
	m_Cells.clear();
}

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

	BuildNavPortals(cellDatas);

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

ASTAR_RESULT NavMesh::FindPath(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex)
{
	ASTAR_RESULT result{};

	result = AStar(startPosition, startCellIndex, targetPosition, targetCellIndex);

	return result;
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

void NavMesh::BuildNavPortals(std::vector<NAVCELL_DATA>& datas)
{
	for (_uint i = 0; i < datas.size(); ++i)
	{
		NAVCELL_DATA cellDataA = datas[i];
		for (_uint j = 0; j < 3; ++j)
		{
			if (-1 == cellDataA.neighbors[j])
				continue;
			if (cellDataA.neighbors[j] <= i)
				continue;

			NAV_PORTAL portal{};
			NAVCELL_DATA cellDataB = datas[cellDataA.neighbors[j]];

			portal.cellA = cellDataA.index;
			portal.cellB = cellDataB.index;
			
			if (0 == j)
			{
				portal.leftPoint = cellDataA.points[0];
				portal.rightPoint = cellDataA.points[1];
			}
			else if (1 == j)
			{
				portal.leftPoint = cellDataA.points[1];
				portal.rightPoint = cellDataA.points[2];
			}
			else
			{
				portal.leftPoint = cellDataA.points[2];
				portal.rightPoint = cellDataA.points[0];
			}

			_uint portalIndex = m_NavPortals.size();
			m_NavPortals.push_back(portal);

			m_Cells[cellDataA.index]->SetPortalIndex(cellDataB.index, portalIndex);
			m_Cells[cellDataB.index]->SetPortalIndex(cellDataA.index, portalIndex);
		}
	}
}

ASTAR_RESULT NavMesh::AStar(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex)
{
	ASTAR_RESULT result{};

	if (startCellIndex >= m_Cells.size() || targetCellIndex >= m_Cells.size())
		return result;

	/*시작점 == 목표지점*/
	if (startCellIndex == targetCellIndex)
	{
		result.isAvailable = true;
		result.cellIndices.push_back(startCellIndex);
		result.centerPoints.push_back(m_Cells[startCellIndex]->GetPositionInCell());
		result.targetPosition = targetPosition;

		return result;
	}

	std::vector<ASTAR_HISTORY> histories(m_Cells.size());
	std::priority_queue<ASTAR_NODE, std::vector<ASTAR_NODE>, ASTAR_COMPARE> open;

	/*init open*/
	histories[startCellIndex].parent = startCellIndex;
	histories[startCellIndex].gCost = 0;
	histories[startCellIndex].fCost = Heuristic(startCellIndex, targetCellIndex);
	open.push({ startCellIndex,histories[startCellIndex].fCost });

	while (!open.empty())
	{
		_uint currCellIndex = open.top().cellIndex;
		_float currFCost = open.top().fCost;
		open.pop();

		if (histories[currCellIndex].closed)
			continue; //확정된 셀
		if (histories[currCellIndex].fCost != currFCost)
			continue;//이미 더 좋은 비용으로 갱신됨

		histories[currCellIndex].closed = true;	//방문 완료

		if (currCellIndex == targetCellIndex)
			break;//목표 도착
		
		for (_uint i = 0; i < 3; ++i)
		{
			_int nextCellIndex = m_Cells[currCellIndex]->GetNeighborIndex(i);
			if (-1 == nextCellIndex)
				continue;
			if (histories[nextCellIndex].closed == true)
				continue;

			_float3 currCellPosition = m_Cells[currCellIndex]->GetPositionInCell();
			_float3 nextCellPosition = m_Cells[nextCellIndex]->GetPositionInCell();

			_float gCost = histories[currCellIndex].gCost + XMVectorGetX(XMVector3Length(XMLoadFloat3(&nextCellPosition)-XMLoadFloat3(&currCellPosition)));
			if (gCost < histories[nextCellIndex].gCost)
			{
				histories[nextCellIndex].gCost = gCost;
				histories[nextCellIndex].fCost = gCost + Heuristic(nextCellIndex, targetCellIndex);
				histories[nextCellIndex].parent = currCellIndex;
				open.push({ static_cast<_uint>(nextCellIndex),histories[nextCellIndex].fCost });
			}
		}
	}

	std::vector<_uint> pathIndices;
	BuildPathCellIndices(histories, pathIndices, startCellIndex, targetCellIndex);
	result.isAvailable = true;
	result.cellIndices = std::move(pathIndices);
	result.targetPosition = targetPosition;

	for (_uint i = 0; i < result.cellIndices.size(); ++i)
		result.centerPoints.push_back(m_Cells[result.cellIndices[i]]->GetPositionInCell());

	for (_uint i = 0; i < result.cellIndices.size(); ++i)
	{
		if (i < result.cellIndices.size() - 1)
		{
			_uint currCellIndex = result.cellIndices[i];
			_uint nextCellIndex = result.cellIndices[i + 1];

			_int neighborIndex{};
			for (_uint j = 0; j < 3; ++j)
			{
				neighborIndex = m_Cells[currCellIndex]->GetNeighborIndex(j);
				if (nextCellIndex == neighborIndex)
				{
					_int portalIndex = m_Cells[currCellIndex]->GetPortalIndex(j);
					NAV_PORTAL portal = m_NavPortals[portalIndex];
					portal.leftPoint.y = 0.f;
					portal.rightPoint.y = 0.f;

					if (portal.cellA == currCellIndex)
					{
						result.portals.push_back(portal);
						break;
					}
					else
					{
						std::swap(portal.cellA, portal.cellB);
						std::swap(portal.leftPoint, portal.rightPoint);

						result.portals.push_back(portal);
						break;
					}
				}
			}
		}
	}

	return result;
}

void NavMesh::BuildPathCellIndices(std::vector<ASTAR_HISTORY>& histories, std::vector<_uint>& pathIndices, _uint startCellIndex, _uint targetCellIndex)
{
	_uint cellIndex = targetCellIndex;
	while (true)
	{
		pathIndices.push_back(cellIndex);
		cellIndex = histories[cellIndex].parent;

		/*경로 복원 끝*/
		if (cellIndex == histories[cellIndex].parent)
			break;
	}

	std::reverse(pathIndices.begin(), pathIndices.end());
}

_float NavMesh::Heuristic(_uint cellIndexA, _uint cellIndexB)
{
	auto cellA = m_Cells[cellIndexA];
	auto cellB = m_Cells[cellIndexB];

	_float3 centerA = cellA->GetPositionInCell();
	_float3 centerB = cellB->GetPositionInCell();

	_float3 p0 = cellB->GetPoint(0);
	_float3 p1 = cellB->GetPoint(1);
	_float3 p2 = cellB->GetPoint(2);

	_float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&centerA) - XMLoadFloat3(&centerB)));
	_float size = math::CalculateTriangleArea(p0, p1, p2);

	return distance * size;
}

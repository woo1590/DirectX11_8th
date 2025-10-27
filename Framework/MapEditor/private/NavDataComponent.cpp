#include "MapEditorPCH.h"
#include "NavDataComponent.h"
#include "VIBufferCell.h"
#include "MaterialInstance.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

NavDataComponent::NavDataComponent(Object* owner)
	:Component(owner)
{
}

NavDataComponent::NavDataComponent(const NavDataComponent& prototype)
	:Component(prototype)
{
}

NavDataComponent* NavDataComponent::Create(Object* owner)
{
	NavDataComponent* Instance = new NavDataComponent(owner);

	if (FAILED(Instance->Initialize_Prototype()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT NavDataComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT NavDataComponent::Initialize(InitDESC* arg)
{
	return S_OK;
}

HRESULT NavDataComponent::ExtractRenderProxy(std::vector<RenderProxy>& proxies)
{
	auto transform = m_pOwner->GetComponent<TransformComponent>();

	for (_uint i = 0; i < m_VIBuffers.size(); ++i)
	{
		RenderProxy proxy{};
		proxy.buffer = m_VIBuffers[i];
		proxy.materialInstance = m_MaterialInstances[i];
		proxy.worldMatrix = transform->GetWorldMatrix();

		proxies.push_back(proxy);
	}

	return S_OK;
}

void NavDataComponent::ExportNavData(std::ostream& file)
{
	_uint numNavDatas = m_NavCellDatas.size();

	file.write(reinterpret_cast<const char*>(&numNavDatas), sizeof(_uint));
	file.write(reinterpret_cast<const char*>(m_NavCellDatas.data()), sizeof(NAVCELL_DATA) * numNavDatas);
}

void NavDataComponent::ImportNavData(std::istream& file)
{
	_uint numNavDatas{};
	file.read(reinterpret_cast<char*>(&numNavDatas), sizeof(_uint));

	m_NavCellDatas.resize(numNavDatas);
	file.read(reinterpret_cast<char*>(m_NavCellDatas.data()), sizeof(NAVCELL_DATA) * numNavDatas);

	for (_uint i = 0; i < numNavDatas; ++i)
	{
		auto buffer = VIBufferCell::Create(m_NavCellDatas[i].points);
		auto mtrlInstance = MaterialInstance::Create();

		mtrlInstance->SetFloat3("g_CellPointA", m_NavCellDatas[i].points[0]);
		mtrlInstance->SetFloat3("g_CellPointB", m_NavCellDatas[i].points[1]);
		mtrlInstance->SetFloat3("g_CellPointC", m_NavCellDatas[i].points[2]);
		mtrlInstance->SetInt("g_IsHover", 0);

		m_VIBuffers.push_back(buffer);
		m_MaterialInstances.push_back(mtrlInstance);
	}

	_uint maxPointIndex = 0;
	for (_uint i = 0; i < numNavDatas; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			if (maxPointIndex < m_NavCellDatas[i].pointIndices[j])
				maxPointIndex = m_NavCellDatas[i].pointIndices[j];
		}
	}

	m_iPointIndex = maxPointIndex + 1;
}

void NavDataComponent::SetHoverIndex(_int index)
{
	if (-1 != index && index >= m_NavCellDatas.size())
		return;

	if (-1 == index)
	{
		if (-1 != m_iLastHoverIndex)
			m_MaterialInstances[m_iLastHoverIndex]->SetInt("g_IsHover", 0);
	}
	else
	{
		if (-1 == m_iLastHoverIndex)
		{
			m_iLastHoverIndex = index;
			m_MaterialInstances[m_iLastHoverIndex]->SetInt("g_IsHover", 1);
		}
		else
		{
			m_MaterialInstances[m_iLastHoverIndex]->SetInt("g_IsHover", 0);

			m_iLastHoverIndex = index;
			m_MaterialInstances[m_iLastHoverIndex]->SetInt("g_IsHover", 1);
		}
	}
}

void NavDataComponent::AddNavCell(const _float3 * points)
{
	_float3 clockWisePoints[3]{ points[0],points[1],points[2] };
	_uint pointIndices[3]{ m_iPointIndex++, m_iPointIndex++, m_iPointIndex++ };
	MakeClockWise(clockWisePoints, pointIndices);

	_uint index = m_NavCellDatas.size();

	/*add cell data*/
	NAVCELL_DATA navCellData{};
	navCellData.index = index;
	memcpy_s(navCellData.points, sizeof(_float3) * 3, clockWisePoints, sizeof(_float3) * 3);
	memcpy_s(navCellData.pointIndices, sizeof(_uint) * 3, pointIndices, sizeof(_uint) * 3);
	
	/*AB*/
	_float3 normalAB{ -1.f * (navCellData.points[1].z - navCellData.points[0].z),
							  navCellData.points[1].y - navCellData.points[0].y,
							  navCellData.points[1].x - navCellData.points[0].x };
	XMStoreFloat3(&navCellData.lines[0], XMVector3Normalize(XMLoadFloat3(&normalAB)));

	/*BC*/
	_float3 normalBC{ -1.f * (navCellData.points[2].z - navCellData.points[1].z),
							  navCellData.points[2].y - navCellData.points[1].y,
							  navCellData.points[2].x - navCellData.points[1].x };
	XMStoreFloat3(&navCellData.lines[1], XMVector3Normalize(XMLoadFloat3(&normalBC)));

	/*CA*/
	_float3 normalCA{ -1.f * (navCellData.points[0].z - navCellData.points[2].z),
							  navCellData.points[0].y - navCellData.points[2].y,
							  navCellData.points[0].x - navCellData.points[2].x };
	XMStoreFloat3(&navCellData.lines[2], XMVector3Normalize(XMLoadFloat3(&normalCA)));

	VIBufferCell* bufferCell = VIBufferCell::Create(navCellData.points);
	MaterialInstance* mtrlInstance = MaterialInstance::Create();

	mtrlInstance->SetFloat3("g_CellPointA", points[0]);
	mtrlInstance->SetFloat3("g_CellPointB", points[1]);
	mtrlInstance->SetFloat3("g_CellPointC", points[2]);
	mtrlInstance->SetInt("g_IsHover", 0);
	
	m_NavCellDatas.push_back(navCellData);
	m_VIBuffers.push_back(bufferCell);
	m_MaterialInstances.push_back(mtrlInstance);
}

void NavDataComponent::AddNavCell(_uint index, _float3 point)
{
	NAVCELL_DATA cellData = m_NavCellDatas[index];

	_float3 A = cellData.points[0];
	_float3 B = cellData.points[1];
	_float3 C = cellData.points[2];

	_float distanceA = math::DistancePointToLine(point, cellData.points[0], cellData.points[1]);
	_float distanceB = math::DistancePointToLine(point, cellData.points[1], cellData.points[2]);
	_float distanceC = math::DistancePointToLine(point, cellData.points[2], cellData.points[0]);

	_uint edge = 0;
	_float minDistance = distanceA;
	_uint pointIndices[3]{ m_iPointIndex++ , cellData.pointIndices[0],cellData.pointIndices[1]};
	_float3 p0 = A;
	_float3 p1 = B;

	if (distanceB < minDistance)
	{
		minDistance = distanceB;
		pointIndices[1] = cellData.pointIndices[1];
		pointIndices[2] = cellData.pointIndices[2];
		p0 = B;
		p1 = C;
	}
	if (distanceC < minDistance)
	{
		minDistance = distanceC;
		pointIndices[1] = cellData.pointIndices[2];
		pointIndices[2] = cellData.pointIndices[0];
		p0 = C;
		p1 = A;
	}

	/*point inside cell*/
	_vector normal = XMVector3Cross(XMLoadFloat3(&B) - XMLoadFloat3(&A), XMLoadFloat3(&C) - XMLoadFloat3(&A));
	_vector side = XMVector3Cross(XMLoadFloat3(&p1) - XMLoadFloat3(&p0), XMLoadFloat3(&point) - XMLoadFloat3(&p0));
	_float t = XMVectorGetX(XMVector3Dot(normal, side));
	if (t > 0)
		return;
	
	/*add cell data*/
	_float3 newPoints[3]{ point, p0,p1 };
	MakeClockWise(newPoints, pointIndices);
	_uint newIndex = m_NavCellDatas.size();

	NAVCELL_DATA newCellData{};
	newCellData.index = newIndex;
	memcpy_s(newCellData.points, sizeof(_float3) * 3, newPoints, sizeof(_float3) * 3);
	memcpy_s(newCellData.pointIndices, sizeof(_uint) * 3, pointIndices, sizeof(_uint) * 3);

	/*AB*/
	_float3 normalAB{ -1.f * (newCellData.points[1].z - newCellData.points[0].z),
							  newCellData.points[1].y - newCellData.points[0].y,
							  newCellData.points[1].x - newCellData.points[0].x };
	XMStoreFloat3(&newCellData.lines[0], XMVector3Normalize(XMLoadFloat3(&normalAB)));

	/*BC*/
	_float3 normalBC{ -1.f * (newCellData.points[2].z - newCellData.points[1].z),
							  newCellData.points[2].y - newCellData.points[1].y,
							  newCellData.points[2].x - newCellData.points[1].x };
	XMStoreFloat3(&newCellData.lines[1], XMVector3Normalize(XMLoadFloat3(&normalBC)));

	/*CA*/
	_float3 normalCA{ -1.f * (newCellData.points[0].z - newCellData.points[2].z),
							  newCellData.points[0].y - newCellData.points[2].y,
							  newCellData.points[0].x - newCellData.points[2].x };
	XMStoreFloat3(&newCellData.lines[2], XMVector3Normalize(XMLoadFloat3(&normalCA)));

	VIBufferCell* bufferCell = VIBufferCell::Create(newCellData.points);
	MaterialInstance* mtrlInstance = MaterialInstance::Create();

	mtrlInstance->SetFloat3("g_CellPointA", newPoints[0]);
	mtrlInstance->SetFloat3("g_CellPointB", newPoints[1]);
	mtrlInstance->SetFloat3("g_CellPointC", newPoints[2]);
	mtrlInstance->SetInt("g_IsHover", 0);

	m_NavCellDatas.push_back(newCellData);
	m_VIBuffers.push_back(bufferCell);
	m_MaterialInstances.push_back(mtrlInstance);

	MakeNeighbor(index, newIndex);
}

void NavDataComponent::AddNavCell(_uint index1, _uint index2)
{
	NAVCELL_DATA cellDataA = m_NavCellDatas[index1];
	NAVCELL_DATA cellDataB = m_NavCellDatas[index2];

	_int sharedIndex = -1;
	_float3 sharedPoint{};
	std::unordered_set<_uint> indicesA{ cellDataA.pointIndices[0],cellDataA.pointIndices[1] ,cellDataA.pointIndices[2] };
	for (_uint i = 0; i < 3; ++i)
	{
		if (indicesA.find(cellDataB.pointIndices[i]) != indicesA.end())
		{
			sharedIndex = cellDataB.pointIndices[i];
			sharedPoint = cellDataB.points[i];
			break;
		}
	}

	if (-1 == sharedIndex)
	{
		_float3 centerPositionB{};
		XMStoreFloat3(&centerPositionB, (XMLoadFloat3(&cellDataB.points[0]) + XMLoadFloat3(&cellDataB.points[1]) + XMLoadFloat3(&cellDataB.points[2])) / 3.f);

		_float minDistance;
		_float distanceA, distanceB, distanceC;
		_uint points[3];
		distanceA = math::DistancePointToLine(centerPositionB, cellDataA.points[0], cellDataA.points[1]);
		distanceB = math::DistancePointToLine(centerPositionB, cellDataA.points[1], cellDataA.points[2]);
		distanceC = math::DistancePointToLine(centerPositionB, cellDataA.points[2], cellDataA.points[0]);

		minDistance = distanceA;
		points[0] = cellDataA.pointIndices[0];
		points[1] = cellDataA.pointIndices[1];
		if (minDistance > distanceB)
		{
			minDistance = distanceB;
			points[0] = cellDataA.pointIndices[1];
			points[1] = cellDataA.pointIndices[2];
		}
		if (minDistance > distanceC)
		{
			minDistance = distanceC;
			points[0] = cellDataA.pointIndices[2];
			points[1] = cellDataA.pointIndices[0];
		}

		_uint pointIndex{};
		minDistance = FLT_MAX;
		for (_uint i = 0; i < 3; ++i)
		{
			_float distance{};
			distance = math::DistancePointToLine(FindPointFromPointIndex(cellDataB.pointIndices[i]),
												 FindPointFromPointIndex(points[0]),
												 FindPointFromPointIndex(points[1]));

			if (distance < minDistance)
			{
				minDistance = distance;
				points[2] = cellDataB.pointIndices[i];
			}
		}
		ConnectCellToPoint(index1, points[0], points[1], points[2]);
		AddNavCell(index2, m_NavCellDatas.back().index);

	}
	else
	{
		std::vector<std::pair<_uint, _uint>> lines;
		std::vector<_uint> availablePoints;
		for (_uint i = 0; i < 3; ++i)
		{
			if (cellDataA.pointIndices[i] != sharedIndex)
				lines.push_back({ sharedIndex,cellDataA.pointIndices[i] });

			if (cellDataB.pointIndices[i] != sharedIndex)
				lines.push_back({ sharedIndex,cellDataB.pointIndices[i] });
		}


		for (_uint i = 0; i < lines.size(); ++i)
		{
			_uint count = 0;
			for (const auto& cell : m_NavCellDatas)
			{
				std::unordered_set<_uint> existPoints{ cell.pointIndices[0],cell.pointIndices[1],cell.pointIndices[2] };

				if (existPoints.find(lines[i].first) != existPoints.end() && existPoints.find(lines[i].second) != existPoints.end())
					++count;
			}

			if (count < 2)
				availablePoints.push_back(lines[i].second);
		}

		if (availablePoints.size() < 2)
			return;

		/*add new cell*/
		_float3 newPoints[3]{ sharedPoint,FindPointFromPointIndex(availablePoints[0]),FindPointFromPointIndex(availablePoints[1]) };
		_uint newPointIndices[3]{ sharedIndex,availablePoints[0],availablePoints[1] };
		MakeClockWise(newPoints, newPointIndices);
		_uint newIndex = m_NavCellDatas.size();

		NAVCELL_DATA newCellData{};
		newCellData.index = newIndex;
		memcpy_s(newCellData.points, sizeof(_float3) * 3, newPoints, sizeof(_float3) * 3);
		memcpy_s(newCellData.pointIndices, sizeof(_uint) * 3, newPointIndices, sizeof(_uint) * 3);

		/*AB*/
		_float3 normalAB{ -1.f * (newCellData.points[1].z - newCellData.points[0].z),
								  newCellData.points[1].y - newCellData.points[0].y,
								  newCellData.points[1].x - newCellData.points[0].x };
		XMStoreFloat3(&newCellData.lines[0], XMVector3Normalize(XMLoadFloat3(&normalAB)));

		/*BC*/
		_float3 normalBC{ -1.f * (newCellData.points[2].z - newCellData.points[1].z),
								  newCellData.points[2].y - newCellData.points[1].y,
								  newCellData.points[2].x - newCellData.points[1].x };
		XMStoreFloat3(&newCellData.lines[1], XMVector3Normalize(XMLoadFloat3(&normalBC)));

		/*CA*/
		_float3 normalCA{ -1.f * (newCellData.points[0].z - newCellData.points[2].z),
								  newCellData.points[0].y - newCellData.points[2].y,
								  newCellData.points[0].x - newCellData.points[2].x };
		XMStoreFloat3(&newCellData.lines[2], XMVector3Normalize(XMLoadFloat3(&normalCA)));

		VIBufferCell* bufferCell = VIBufferCell::Create(newCellData.points);
		MaterialInstance* mtrlInstance = MaterialInstance::Create();

		mtrlInstance->SetFloat3("g_CellPointA", newPoints[0]);
		mtrlInstance->SetFloat3("g_CellPointB", newPoints[1]);
		mtrlInstance->SetFloat3("g_CellPointC", newPoints[2]);
		mtrlInstance->SetInt("g_IsHover", 0);

		m_NavCellDatas.push_back(newCellData);
		m_VIBuffers.push_back(bufferCell);
		m_MaterialInstances.push_back(mtrlInstance);

		MakeNeighbor(index1, newIndex);
		MakeNeighbor(index2, newIndex);
	}
}

void NavDataComponent::ConnectCellToPoint(_uint cellIndex,_uint pointIndex0, _uint pointIndex1, _uint pointIndex2)
{
	_float3 points[3];
	_uint pointIndices[3];
	points[0] = FindPointFromPointIndex(pointIndex0);
	points[1] = FindPointFromPointIndex(pointIndex1);
	points[2] = FindPointFromPointIndex(pointIndex2);

	pointIndices[0] = pointIndex0;
	pointIndices[1] = pointIndex1;
	pointIndices[2] = pointIndex2;
	
	MakeClockWise(points, pointIndices);

	/*add cell data*/
	_uint index = m_NavCellDatas.size();

	NAVCELL_DATA navCellData{};
	navCellData.index = index;
	memcpy_s(navCellData.points, sizeof(_float3) * 3, points, sizeof(_float3) * 3);
	memcpy_s(navCellData.pointIndices, sizeof(_uint) * 3, pointIndices, sizeof(_uint) * 3);

	/*AB*/
	_float3 normalAB{ -1.f * (navCellData.points[1].z - navCellData.points[0].z),
							  navCellData.points[1].y - navCellData.points[0].y,
							  navCellData.points[1].x - navCellData.points[0].x };
	XMStoreFloat3(&navCellData.lines[0], XMVector3Normalize(XMLoadFloat3(&normalAB)));

	/*BC*/
	_float3 normalBC{ -1.f * (navCellData.points[2].z - navCellData.points[1].z),
							  navCellData.points[2].y - navCellData.points[1].y,
							  navCellData.points[2].x - navCellData.points[1].x };
	XMStoreFloat3(&navCellData.lines[1], XMVector3Normalize(XMLoadFloat3(&normalBC)));

	/*CA*/
	_float3 normalCA{ -1.f * (navCellData.points[0].z - navCellData.points[2].z),
							  navCellData.points[0].y - navCellData.points[2].y,
							  navCellData.points[0].x - navCellData.points[2].x };
	XMStoreFloat3(&navCellData.lines[2], XMVector3Normalize(XMLoadFloat3(&normalCA)));

	VIBufferCell* bufferCell = VIBufferCell::Create(navCellData.points);
	MaterialInstance* mtrlInstance = MaterialInstance::Create();

	mtrlInstance->SetFloat3("g_CellPointA", points[0]);
	mtrlInstance->SetFloat3("g_CellPointB", points[1]);
	mtrlInstance->SetFloat3("g_CellPointC", points[2]);
	mtrlInstance->SetInt("g_IsHover", 0);

	m_NavCellDatas.push_back(navCellData);
	m_VIBuffers.push_back(bufferCell);
	m_MaterialInstances.push_back(mtrlInstance);

	MakeNeighbor(cellIndex, index);
}

void NavDataComponent::LinkCell(_uint index1, _uint index2)
{
	NAVCELL_DATA& cellDataA = m_NavCellDatas[index1];
	NAVCELL_DATA& cellDataB = m_NavCellDatas[index2];
	_uint linkLine{};
	_float dot0, dot1, dot2, maxDot;

	_float3 dir{};
	_float3 centerA, centerB;
	XMStoreFloat3(&centerA, (XMLoadFloat3(&cellDataA.points[0]) + XMLoadFloat3(&cellDataA.points[1]) + XMLoadFloat3(&cellDataA.points[2])) / 3.f);
	XMStoreFloat3(&centerB, (XMLoadFloat3(&cellDataB.points[0]) + XMLoadFloat3(&cellDataB.points[1]) + XMLoadFloat3(&cellDataB.points[2])) / 3.f);
	XMStoreFloat3(&dir, XMVector3Normalize(XMLoadFloat3(&centerB) - XMLoadFloat3(&centerA)));

	dot0 = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&cellDataA.lines[0]), XMLoadFloat3(&dir)));
	dot1 = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&cellDataA.lines[1]), XMLoadFloat3(&dir)));
	dot2 = XMVectorGetX(XMVector3Dot(XMLoadFloat3(&cellDataA.lines[2]), XMLoadFloat3(&dir)));

	maxDot = dot0;
	linkLine = 0;
	if (dot1 > maxDot)
	{
		maxDot = dot1;
		linkLine = 1;
	}
	if (dot2 > maxDot)
	{
		maxDot = dot2;
		linkLine = 2;
	}
	
	cellDataA.linkedCells[linkLine] = index2;
}

void NavDataComponent::ReomoveLastCell()
{
	if (m_NavCellDatas.empty())
		return;

	NAVCELL_DATA lastCellData = m_NavCellDatas.back();

	_uint index = lastCellData.index;
	for (_uint i = 0; i < 3; ++i)
	{
		if (-1 != lastCellData.neighbors[i])
		{
			NAVCELL_DATA& neighborData = m_NavCellDatas[lastCellData.neighbors[i]];
			for (_uint j = 0; j < 3; ++j)
			{
				if (neighborData.neighbors[j] == index)
				{
					neighborData.neighbors[j] = -1;
					break;
				}
			}
		}
	}

	_uint maxPointIndex = 0;
	for (_uint i = 0; i < m_NavCellDatas.size(); ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			if (maxPointIndex < m_NavCellDatas[i].pointIndices[j])
				maxPointIndex = m_NavCellDatas[i].pointIndices[j];
		}
	}
	m_iPointIndex = maxPointIndex;
	m_iLastHoverIndex = -1;

	auto buffer = m_VIBuffers.back();
	auto materialInstance = m_MaterialInstances.back();

	Safe_Release(buffer);
	Safe_Release(materialInstance);

	m_NavCellDatas.pop_back();
	m_VIBuffers.pop_back();
	m_MaterialInstances.pop_back();
}

void NavDataComponent::Free()
{
	__super::Free();

	for (auto& buffer : m_VIBuffers)
		Safe_Release(buffer);
	m_VIBuffers.clear();

	for (auto& mtrl : m_MaterialInstances)
		Safe_Release(mtrl);
	m_MaterialInstances.clear();
}

void NavDataComponent::RenderInspector()
{
	ImGui::PushID(this);
	if (!ImGui::CollapsingHeader("NavData",
		ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding))
	{
		if (-1 != m_iLastHoverIndex)
		{
			auto cell = m_NavCellDatas[m_iLastHoverIndex];
			ImGui::Text("Index : %d", cell.index);
			ImGui::Text("Point Index : %d    %d   %d", cell.pointIndices[0], cell.pointIndices[1], cell.pointIndices[2]);
			ImGui::Text("Neighbor : %d   %d   %d", cell.neighbors[0], cell.neighbors[1], cell.neighbors[2]);
		}

		if (ImGui::CollapsingHeader("Cells"))
		{
			for (const auto& cell : m_NavCellDatas)
			{
				ImGui::Separator();
				ImGui::Text("Index : %d", cell.index);
				ImGui::Text("Point Index : %d    %d   %d", cell.pointIndices[0], cell.pointIndices[1], cell.pointIndices[2]);
				ImGui::Text("Neighbor : %d   %d   %d", cell.neighbors[0], cell.neighbors[1], cell.neighbors[2]);
			}
		}
	}

	ImGui::PopID();
}

void NavDataComponent::MakeClockWise(_float3* points, _uint* pointIndices)
{
	_vector a = XMLoadFloat3(&points[0]);
	_vector b = XMLoadFloat3(&points[1]);
	_vector c = XMLoadFloat3(&points[2]);

	_vector worldUp = XMVectorSet(0.f, 1.f, 0.f, 1.f);
	_vector normal = XMVector3Cross(b - a, c - b);

	_float t = XMVectorGetX(XMVector3Dot(worldUp, normal));

	if (t < 0)
	{
		std::swap(points[1], points[2]);
		std::swap(pointIndices[1], pointIndices[2]);
	}
}

void NavDataComponent::MakeNeighbor(_uint index1, _uint index2)
{
	NAVCELL_DATA& cellDataA = m_NavCellDatas[index1];
	NAVCELL_DATA& cellDataB = m_NavCellDatas[index2];

	for (_uint i = 0; i < 3; ++i)
	{
		std::unordered_set<_uint> lineA{ cellDataA.pointIndices[i],cellDataA.pointIndices[(i + 1) % 3] };
		for (_uint j = 0; j < 3; ++j)
		{
			std::unordered_set<_uint> lineB{ cellDataB.pointIndices[j], cellDataB.pointIndices[(j + 1) % 3] };

			if (lineA == lineB)
			{
				cellDataA.neighbors[i] = index2;
				cellDataB.neighbors[j] = index1;
			}
		}
	}
}

_float3 NavDataComponent::FindPointFromPointIndex(_uint pointIndex)
{
	_float3 point{ -FLT_MAX,-FLT_MAX,-FLT_MAX };
	for (const auto& cell : m_NavCellDatas)
	{
		for (_uint i = 0; i < 3; ++i)
		{
			if (pointIndex == cell.pointIndices[i])
				point = cell.points[i];
		}
	}

	return point;
}


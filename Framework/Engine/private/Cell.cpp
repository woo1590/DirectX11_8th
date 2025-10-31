#include "EnginePCH.h"
#include "Cell.h"
#include "VIBufferCell.h"
#include "MaterialInstance.h"

Cell::Cell()
{
}

Cell* Cell::Create(NAVCELL_DATA data)
{
	Cell* Instance = new Cell();

	if (FAILED(Instance->Initialize(data)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cell::Initialize(NAVCELL_DATA data)
{
	m_iIndex = data.index;
	memcpy_s(m_Points, sizeof(_float3) * ENUM_CLASS(NavCellPoint::Count), data.points, sizeof(_float3) * ENUM_CLASS(NavCellPoint::Count));
	memcpy_s(m_LineNormals, sizeof(_float3) * ENUM_CLASS(NavCellLine::Count), data.lines, sizeof(_float3) * ENUM_CLASS(NavCellLine::Count));
	memcpy_s(m_NeighborCellIndices, sizeof(_int) * ENUM_CLASS(NavCellPoint::Count), data.neighbors, sizeof(_int) * ENUM_CLASS(NavCellPoint::Count));
	memcpy_s(m_LinkedCellIndices, sizeof(_int) * ENUM_CLASS(NavCellPoint::Count), data.linkedCells, sizeof(_int) * ENUM_CLASS(NavCellPoint::Count));

	/*for debug*/
	m_pBuffer = VIBufferCell::Create(m_Points);
	if (!m_pBuffer)
	{
		MSG_BOX("Failed to create : VIBufferCell");
		return E_FAIL;
	}

	m_pMaterialInstance = MaterialInstance::Create();
	if (!m_pMaterialInstance)
	{
		MSG_BOX("Failed to create : MaterialInstance");
		return E_FAIL;
	}

	m_pMaterialInstance->SetFloat3("g_CellPointA", m_Points[0]);
	m_pMaterialInstance->SetFloat3("g_CellPointB", m_Points[1]);
	m_pMaterialInstance->SetFloat3("g_CellPointC", m_Points[2]);

	return S_OK;
}

_float3 Cell::GetCellNormal()
{
	_float3 cellNormal{};
	_vector AB = XMVector3Normalize(XMLoadFloat3(&m_Points[1]) - XMLoadFloat3(&m_Points[0]));
	_vector AC = XMVector3Normalize(XMLoadFloat3(&m_Points[2]) - XMLoadFloat3(&m_Points[0]));

	XMStoreFloat3(&cellNormal, XMVector3Normalize(XMVector3Cross(AB, AC)));

	return cellNormal;
}

_float3 Cell::GetPositionInCell() const
{
	_float3 centerPosition{};

	centerPosition.x = (m_Points[0].x + m_Points[1].x + m_Points[2].x) / 3.f;
	centerPosition.y = (m_Points[0].y + m_Points[1].y + m_Points[2].y) / 3.f;
	centerPosition.z = (m_Points[0].z + m_Points[1].z + m_Points[2].z) / 3.f;

	return centerPosition;
}

_bool Cell::IsLinked(_float3 position, _uint& currCellIndex)
{
	for (_uint i = 0; i < 3; ++i)
	{
		if (-1 != m_LinkedCellIndices[i])
		{
			_vector normal = XMLoadFloat3(&m_LineNormals[i]);
			_vector dir = XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&m_Points[i]));

			_float t = XMVectorGetX(XMVector3Dot(normal, dir));
			if (t > 0)
			{
				currCellIndex = m_LinkedCellIndices[i];
				return true;
			}
		}
	}

	return false;
}

_bool Cell::IsInCell(_float3 position, _int& neighborIndex)
{
	for (_uint i = 0; i < ENUM_CLASS(NavCellLine::Count); ++i)
	{
		_vector normal = XMLoadFloat3(&m_LineNormals[i]);
		_vector dir = XMVector3Normalize(XMLoadFloat3(&position) - XMLoadFloat3(&m_Points[i]));
		
		_float t = XMVectorGetX(XMVector3Dot(normal, dir));
		if (t > 0)
		{
			neighborIndex = m_NeighborCellIndices[i];
			return false;
		}
	}

	return true;
}

_float Cell::GetHeight(_float3 position)
{
	_float4 plane;
	XMStoreFloat4(&plane, XMPlaneFromPoints(XMLoadFloat3(&m_Points[ENUM_CLASS(NavCellPoint::A)]),
											XMLoadFloat3(&m_Points[ENUM_CLASS(NavCellPoint::B)]),
											XMLoadFloat3(&m_Points[ENUM_CLASS(NavCellPoint::C)])));

	//ax + by + cz + d =0;
	//y = -1(ax+cz+d)/b;
	_float height = -1.f * (plane.x * position.x + plane.z * position.z + plane.w) / plane.y;

	return height;
}

_float3 Cell::MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellindex)
{
	_int lineIndex = 0;
	_float minDistance = FLT_MAX;

	_float d0, d1, d2;
	d0 = math::DistancePointToLine(nextPosition, m_Points[0], m_Points[1]);
	d1 = math::DistancePointToLine(nextPosition, m_Points[1], m_Points[2]);
	d2 = math::DistancePointToLine(nextPosition, m_Points[2], m_Points[0]);

	minDistance = d0;
	if (d1 < minDistance)
	{
		minDistance = d1;
		lineIndex = 1;
	}
	if (d2 < minDistance)
	{
		minDistance = d2;
		lineIndex = 2;
	}

	_vector normal = XMLoadFloat3(&m_LineNormals[lineIndex]);
	_vector velocity = XMLoadFloat3(&nextPosition) - XMLoadFloat3(&position);
	_float3 slideVector{};

	if (-1 != m_NeighborCellIndices[lineIndex])
	{
		currCellindex = m_NeighborCellIndices[lineIndex];
		return nextPosition;
	}
	else
	{
		_float t = XMVectorGetX(XMVector3Dot(normal, velocity));

		XMStoreFloat3(&slideVector, velocity - t * normal);
		XMStoreFloat3(&nextPosition, XMLoadFloat3(&position) + XMLoadFloat3(&slideVector));

		return nextPosition;
	}
}

void Cell::Free()
{
	__super::Free();

	Safe_Release(m_pBuffer);
	Safe_Release(m_pMaterialInstance);
}

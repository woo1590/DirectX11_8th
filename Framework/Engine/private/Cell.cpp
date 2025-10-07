#include "EnginePCH.h"
#include "Cell.h"

Cell::Cell()
{
}

Cell* Cell::Create(CELL_DESC desc)
{
	Cell* Instance = new Cell();

	if (FAILED(Instance->Initialize(desc)))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Cell::Initialize(CELL_DESC desc)
{
	memcpy_s(m_Points, sizeof(_float3) * ENUM_CLASS(CellPoint::Count), desc.points, sizeof(_float3) * ENUM_CLASS(CellPoint::Count));
	m_iIndex = desc.index;

	return S_OK;
}

void Cell::Free()
{
	__super::Free();
}

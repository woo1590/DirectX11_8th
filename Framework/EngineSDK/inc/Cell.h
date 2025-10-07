#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Cell :
    public Base
{
private:
    Cell();
    virtual ~Cell() = default;

public:
    static Cell* Create(CELL_DESC desc);
    HRESULT Initialize(CELL_DESC desc);

    void Free()override;
private:
    _uint m_iIndex{};
    _int m_NeighborCellIndices[3] = { -1,-1,-1 };
    _float3 m_Points[ENUM_CLASS(CellPoint::Count)]{};
};

NS_END
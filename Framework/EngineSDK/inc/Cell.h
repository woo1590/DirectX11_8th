#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class VIBuffer;
class MaterialInstance;
class ENGINE_DLL Cell :
    public Base
{
private:
    Cell();
    virtual ~Cell() = default;

public:
    static Cell* Create(NAVCELL_DATA desc);
    HRESULT Initialize(NAVCELL_DATA desc);

    /*getter*/
    VIBuffer* GetBuffer()const { return m_pBuffer; }
    MaterialInstance* GetMaterialInstance()const { return m_pMaterialInstance; }

    /*API*/
    _float3 GetPositionInCell()const;
    _bool IsLinked(_float3 poisition, _uint& currCellIndex);
    _bool IsInCell(_float3 position, _int& neighborIndex);
    _float GetHeight(_float3 position);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellindex);

    void Free()override;
private:
    _uint m_iIndex{};
    _float3 m_Points[ENUM_CLASS(NavCellPoint::Count)]{};
    _float3 m_LineNormals[ENUM_CLASS(NavCellLine::Count)]{};
    _int m_NeighborCellIndices[3] = { -1,-1,-1 };
    _int m_LinkedCellIndices[3] = { -1,-1,-1, };

    /*for debug*/
    VIBuffer* m_pBuffer = nullptr;
    MaterialInstance* m_pMaterialInstance = nullptr;
};

NS_END
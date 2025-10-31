#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class Cell;
class ENGINE_DLL NavMesh :
    public Base
{
private:
    NavMesh();
    virtual ~NavMesh() = default;

public:
    static NavMesh* Create(const _string& filePath);
    HRESULT Initialize(const _string& filePath);

    /*for debug*/
    HRESULT ExtractDebugProxies(std::vector<RenderProxy>& proxies);

    /*API*/
    _float3 GetCellNormal(_uint cellIndex);
    _bool IsCellExist(_uint cellIndex);
    _float3 GetPositionInCell(_uint cellIndex);
    _bool IsLinkedCell(_float3 position, _uint& currCellIndex);
    _bool IsMove(_float3 position, _uint& currCellIndex);
    _float GetHeight(_float3 position, _uint currCellIndex);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellIndex);

    void Free()override;

private:
    _uint m_iNumCells{};
    std::vector<Cell*> m_Cells;
};

NS_END
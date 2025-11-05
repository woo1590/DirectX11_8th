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

    _float3 GetPositionInCell(_uint cellIndex);
    _float GetHeight(_float3 position, _uint currCellIndex);
      
    /*API*/
    ASTAR_RESULT FindPath(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex);
    _float3 GetCellNormal(_uint cellIndex);
    _bool IsCellExist(_uint cellIndex);
    _bool IsLinkedCell(_float3 position, _uint& currCellIndex);
    _bool IsMove(_float3 position, _uint& currCellIndex);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellIndex);

    void Free()override;

private:
    void BuildNavPortals(std::vector<NAVCELL_DATA>& datas);
    ASTAR_RESULT AStar(_float3 startPosition, _uint startCellIndex, _float3 targetPosition, _uint targetCellIndex);
    void BuildPathCellIndices(std::vector<ASTAR_HISTORY>& histories, std::vector<_uint>& pathIndices, _uint startCellIndex, _uint targetCellIndex);
    _float Heuristic(_uint cellIndexA, _uint cellIndexB);

    _uint m_iNumCells{};
    std::vector<Cell*> m_Cells;
    std::vector<NAV_PORTAL> m_NavPortals;
};

NS_END
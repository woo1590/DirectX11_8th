#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class VIBufferCell;
class MaterialInstance;
NS_END

NS_BEGIN(MapEditor)

class NavDataComponent :
    public Component
{
private:
    NavDataComponent(Object* owner);
    NavDataComponent(const NavDataComponent& prototype);
    virtual ~NavDataComponent() = default;

public:
    static NavDataComponent* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;

    HRESULT ExtractRenderProxy(std::vector<RenderProxy>& proxies);
    void ExportNavData(std::ostream& file);
    void ImportNavData(std::istream& file);

    void SetHoverIndex(_int index);

    void AddNavCell(const _float3 * points);     /*new cell : 3 points*/
    void AddNavCell(_uint index, _float3 point); /*new cell : 1 cell, 1 point*/
    void AddNavCell(_uint index1, _uint index2); /*new cell : 2 cells*/
    void ReomoveLastCell();

    /*Getter*/
    const std::vector<NAVCELL_DATA>& GetNavCellDatas() { return m_NavCellDatas; }
    std::vector<MaterialInstance*>& GetMaterialInstances() { return m_MaterialInstances; }

    Component* Clone()override { return new NavDataComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override;
#endif

private:
    void MakeClockWise(_float3* points, _uint* pointIndices);
    void MakeNeighbor(_uint index1, _uint index2);
    _float3 FindPointFromPointIndex(_uint pointIndex);

    _uint m_iPointIndex{};
    std::vector<NAVCELL_DATA> m_NavCellDatas;
    std::vector<VIBufferCell*> m_VIBuffers;
    std::vector<MaterialInstance*> m_MaterialInstances;

    _int m_iLastHoverIndex = -1;
};

NS_END
#pragma once
#include "Component.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)
class VIBufferCell;
class MaterialInstance;
NS_END

NS_BEGIN(MapEditor)

typedef struct tagNavCellData
{
    _uint index{};
    _float3 points[3];  /*world position*/
    _int neighbors[3]{ -1,-1,-1 };
    std::pair<_uint, _uint> edges[3]; /*point index*/
}NAVCELL_DATA;

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

    void AddNavCell(const _float3* points);/*add cell 3 points*/

    /*Getter*/
    const std::vector<NAVCELL_DATA>& GetNavCellDatas() { return m_NavCellDatas; }

    Component* Clone()override { return new NavDataComponent(*this); }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector()override {};
#endif

private:
    _uint m_iPointIndex{};
    std::vector<NAVCELL_DATA> m_NavCellDatas;
    std::vector<VIBufferCell*> m_VIBuffers;
    std::vector<MaterialInstance*> m_MaterialInstances;
};

NS_END
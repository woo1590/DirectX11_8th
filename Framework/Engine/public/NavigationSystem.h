#pragma once
#include "Base.h"
#include "RenderProxy.h"

NS_BEGIN(Engine)

class NavMesh;
class NavigationComponent;
class NavigationSystem :
    public Base
{
private:
    NavigationSystem();
    virtual ~NavigationSystem() = default;

public:
    static NavigationSystem* Create();
    HRESULT Initialize();

    HRESULT SetNavMesh(_uint levelID, const _string& navMeshTag);
    void RegisterNavigation(NavigationComponent* component);

    /*API*/
    _bool IsCellExist(_uint cellIndex);
    _float3 GetPositionInCell(_uint cellIndex);
    _bool IsLinkedCell(_float3 position, _uint& currCellIndex);
    _bool IsMove(_float3 position, _uint& currCellIndex);
    _float GetHeight(_float3 position, _uint currCellIndex);
    _float3 MakeSlideVector(_float3 position, _float3 nextPosition, _uint& currCellIndex);

    HRESULT ExtractDebugProxies(std::vector<RenderProxy>& proxies);

    void Free()override;

private:
    NavMesh* m_pNavMesh = nullptr;
};

NS_END
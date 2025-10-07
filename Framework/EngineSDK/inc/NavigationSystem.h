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

    void RegisterNavigation(NavigationComponent* component);
    void UnRegisterNavigation(NavigationComponent* component);
    HRESULT ExtractDebugProxies(std::vector<RenderProxy>& proxies);

    void Free()override;

private:
    std::list<NavigationComponent*> m_NavComponents;
    NavMesh* m_pNavMesh = nullptr;
};

NS_END
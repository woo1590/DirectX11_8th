#pragma once
#include "Base.h"
#include "LightProxy.h"

NS_BEGIN(Engine)

class LightComponent;
class LightManager :
    public Base
{
private:
    LightManager();
    virtual ~LightManager() = default;

public:
    static LightManager* Create();
    HRESULT Initialize();

    void RegisterLight(LightComponent* light);
    void UnRegisterLight(LightComponent* light);
    HRESULT ExtractLightProxy(std::vector<LightProxy>& lights);

    void Free()override;

private:
    std::list<LightComponent*> m_Lights;
};

NS_END
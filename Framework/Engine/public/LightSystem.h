#pragma once
#include "Base.h"
#include "LightProxy.h"

NS_BEGIN(Engine)

class LightComponent;
class LightSystem :
    public Base
{
private:
    LightSystem();
    virtual ~LightSystem() = default;

public:
    static LightSystem* Create();
    HRESULT Initialize();

    void RegisterLight(LightComponent* light);
    void UnRegisterLight(LightComponent* light);
    HRESULT ExtractLightProxy(std::vector<LightProxy>& lights);

    void Free()override;

private:
    std::list<LightComponent*> m_Lights;
};

NS_END
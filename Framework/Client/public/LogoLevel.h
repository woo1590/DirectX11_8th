#pragma once
#include "Level.h"

NS_BEGIN(Client)

class LogoLevel :
    public Level
{
private:
    LogoLevel();
    virtual ~LogoLevel() = default;

public:
    static LogoLevel* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    _uint m_iBGMChannelID{};

    HRESULT Initialize_LayerHome(const _string& layerTag);
    HRESULT Initialize_LayerCharacter(const _string& layerTag);
    HRESULT Initialize_LayerCamera(const _string& layerTag);
    HRESULT Initialize_LayerLight(const _string& layerTag);
    HRESULT Initialize_LayerUI(const _string& layerTag);
};

NS_END
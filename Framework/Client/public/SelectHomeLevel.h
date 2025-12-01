#pragma once
#include "Level.h"

NS_BEGIN(Client)

class SelectHomeLevel :
    public Level
{
private:
    SelectHomeLevel();
    virtual ~SelectHomeLevel() = default;

public:
    static SelectHomeLevel* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    _uint m_iBGMChannel{};

    HRESULT Initialize_LayerHome(const _string& layerTag);
    HRESULT Initialize_LayerCharacter(const _string& layerTag);
};

NS_END
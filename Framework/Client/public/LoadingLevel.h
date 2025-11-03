#pragma once
#include "Level.h"

NS_BEGIN(Client)

class Loader;
class LoadingLevel final:
    public Level
{
private:
    LoadingLevel();
    virtual ~LoadingLevel() = default;

public:
    static LoadingLevel* Create(LevelID nextLevelID);
    HRESULT Initialize(LevelID nextLevelID);
    void Free()override;

public:
    void Update(_float dt)override;
    HRESULT Render()override;

private:
    Loader* m_pLoader = nullptr;
    LevelID m_eNextLevelID{};

    HRESULT Initialize_LoadingUI();

};

NS_END
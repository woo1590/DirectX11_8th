#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Level;
class LevelManager final:
    public Base
{
private:
    LevelManager();
    virtual ~LevelManager() = default;

public:
    static LevelManager* Create();
    HRESULT Initialize();
    void Free()override;

public:
    void Update(_float dt);
    HRESULT Render();
    void ChangeLevel(_uint nextLevelID, Level* nextLevel);

private:
    Level* currLevel = nullptr;
    _uint currLevelID{};
};

NS_END
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
    void ChangeLevel(Level* nextLevel);

private:
    Level* currentLevel = nullptr;
};

NS_END
#pragma once
#include "Level.h"

NS_BEGIN(Client)

class GamePlayLevel :
    public Level
{
private:
    GamePlayLevel();
    virtual ~GamePlayLevel() = default;

public:
    static GamePlayLevel* Create();
    HRESULT Initialize();
    void Free()override;  

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    HRESULT IntializeLayerCamera(const _string& layerTag);
    HRESULT InitializeLayerGameObject(const _string& layerTag);
};

NS_END
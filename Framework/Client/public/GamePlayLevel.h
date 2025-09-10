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
    HRESULT Initialize_LayerLights(const _string& layerTag);
    HRESULT Intialize_LayerCamera(const _string& layerTag);
    HRESULT Initialize_LayerGameObject(const _string& layerTag);
};

NS_END
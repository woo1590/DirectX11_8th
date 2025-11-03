#pragma once
#include "Level.h"

NS_BEGIN(Client)

class StageBoss :
    public Level
{
private:
    StageBoss();
    virtual ~StageBoss() = default;

public:
    static StageBoss* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    _bool m_IsBossSpawned = false;

    HRESULT LoadMapFromFile(const _string& filePath);

    HRESULT Initialize_LayerLights(const _string& layerTag);
    HRESULT Initialize_LayerSkybox(const _string& layerTag);
    HRESULT Initialize_LayerPlayer(const _string& layerTag);
    HRESULT Initialize_LayerUI(const _string& layerTag);
};

NS_END
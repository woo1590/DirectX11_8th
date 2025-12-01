#pragma once
#include "Level.h"

NS_BEGIN(Client)

class Stage2 :
    public Level
{
private:
    Stage2();
    virtual ~Stage2() = default;

public:
    static Stage2* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    HRESULT LoadMapFromFile(const _string& filePath);
    HRESULT LoadLightFromFile(const _string& filePath);

    HRESULT Initialize_LayerPlayer(const _string& layerTag);
    HRESULT Initialize_LayerCamera(const _string& layerTag);
    HRESULT Initialize_LayerUI(const _string& layerTag);
    HRESULT Initialize_LayerChest(const _string& layerTag);

};

NS_END
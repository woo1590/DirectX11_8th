#pragma once
#include "Level.h"

NS_BEGIN(Client)

class Stage3 :
    public Level
{
private:
    Stage3();
    virtual ~Stage3() = default;

public:
    static Stage3* Create();
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
#pragma once
#include "Level.h"

NS_BEGIN(Client)

class Stage1 :
    public Level
{
private:
    Stage1();
    virtual ~Stage1() = default;

public:
    static Stage1* Create();
    HRESULT Initialize();
    void Free()override;

    void Update(_float dt)override;
    HRESULT Render()override;

private:
    HRESULT LoadMapFromFile(const _string& filePath);

    HRESULT Initialize_LayerLights(const _string& layerTag);
    HRESULT Initialize_LayerPlayer(const _string& layerTag);
    HRESULT Initialize_LayerUI(const _string& layerTag);
};

NS_END
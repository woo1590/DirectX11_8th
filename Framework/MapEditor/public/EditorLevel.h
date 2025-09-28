#pragma once
#include "Level.h"

NS_BEGIN(MapEditor)

class MapEditorPanel;
class EditorLevel final:
    public Level
{
private:
    EditorLevel();
    virtual ~EditorLevel() = default;

public:
    static EditorLevel* Create();
    HRESULT Initialize();
    void Update(_float dt)override;
    HRESULT Render()override;

    void Free()override;

private:
    HRESULT Initialize_DefaultResource();
    HRESULT Initialize_LayerLight(const _string& layerTag);
    HRESULT Initialize_LayerCamera(const _string& layerTag);
};

NS_END
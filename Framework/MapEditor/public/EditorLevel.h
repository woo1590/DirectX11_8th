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
};

NS_END
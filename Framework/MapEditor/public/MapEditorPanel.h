#pragma once
#include "IPanel.h"

NS_BEGIN(MapEditor)

class MapEditorPanel :
    public IPanel
{
private:
    MapEditorPanel();
    virtual ~MapEditorPanel() = default;

public:
    static MapEditorPanel* Create();
    HRESULT Initialize() override;
    void Draw(GUIState& state) override;

    void Free()override;

private:
    void ImportMapFile();
    void ExportMapFile();
};

NS_END

#pragma once
#include "IPanel.h"
#include "PickingSystem.h"

NS_BEGIN(MapEditor)

class PickingSystem;
class MapEditorPanel :
    public IPanel
{
private:
    MapEditorPanel(PickingSystem* picking);
    virtual ~MapEditorPanel() = default;

public:
    static MapEditorPanel* Create(PickingSystem* picking);
    HRESULT Initialize() override;
    void Draw(GUIState& state) override;

    void LoadPrefabs(const _string& filePath);
    void Free()override;

private:
    void ImportMapFile();
    void ExportMapFile();
    void AddObjectToLayer(PICK_RESULT pickRes);
    void ShowPrefabs();

    _int m_iSelectedIndex = -1;
    _uint m_iNumPrefabs{};
    std::vector<PREFAB> m_Prefabs;

    PickingSystem* m_pPickingSystem = nullptr;
};

NS_END

#pragma once
#include "IPanel.h"
#include "PickingSystem.h"

NS_BEGIN(MapEditor)

class Chunk;
class PickingSystem;
class MapEditorPanel :
    public IPanel
{
    enum class EditMode { Guizmo, Placement, NavPlacement, Count };
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
    void ImportMapFile(const _string& filePath);
    void ExportMapFile(const _string& outFilePath);
    void ShowPrefabs();
    
    /*Placement*/
    void Placement(GUIState& state, PICK_RESULT pickRes);
    void AddObjectToLayer(PICK_RESULT pickRes);
    void DeleteObjectFromLayer(GUIState& state, PICK_RESULT pickRes);
    void Undo();
    void Redo();
    void ShowPreviewObject(PICK_RESULT pickRes);

    /*Nav Placement*/   
    void NavPlacement(GUIState& state, PICK_RESULT pickRes) {};

    _int m_iSelectedIndex = -1;

    _uint m_iPreviewObjectIndex = 0;
    Object* m_pPreviewObject = nullptr;

    _uint m_iNumPrefabs{};
    std::vector<PREFAB> m_Prefabs;

    PickingSystem* m_pPickingSystem = nullptr;
    EditMode m_eMode = EditMode::Placement;
};

NS_END

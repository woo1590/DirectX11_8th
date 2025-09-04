#pragma once
#include "IPanel.h"

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class TransformComponent;
class ImGuiManager;
class GuizmoPanel final:
    public IPanel
{
private:
    GuizmoPanel();
    virtual ~GuizmoPanel() = default;

public:
    static GuizmoPanel* Create();
    HRESULT Initialize();

    void Draw(GUIState& state)override;
    void Free()override;

private:
};

NS_END

#endif
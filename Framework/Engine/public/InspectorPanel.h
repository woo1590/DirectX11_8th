#pragma once
#include "IPanel.h"

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class InspectorPanel final:
    public IPanel
{
private:
    InspectorPanel();
    virtual ~InspectorPanel() = default;

public:
    static InspectorPanel* Create();
    HRESULT Initialize()override;
    void Draw(GUIState& state)override;

    void Free()override;
private:

};

NS_END

#endif
#pragma once
#include "IPanel.h"

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class OutlinerPanel final:
    public IPanel
{
private:
    OutlinerPanel();
    virtual ~OutlinerPanel() = default;

public:
    static OutlinerPanel* Create();
    HRESULT Initialize()override;
    void Draw(GUIState& state)override;
    
    void Free()override;
private:

};

NS_END

#endif
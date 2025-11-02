#pragma once
#include "IPanel.h"

NS_BEGIN(Engine)

class RenderTargetPanel :
    public IPanel
{
private:
    RenderTargetPanel();
    virtual ~RenderTargetPanel() = default;

public:
    static RenderTargetPanel* Create();
    HRESULT Initialize()override;
    void Draw(GUIState& state)override;

    void Free()override;

private:

};
    NS_END
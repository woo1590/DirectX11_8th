#pragma once
#include "IPanel.h"

NS_BEGIN(EffectEditor)

class EffectEditPanel :
    public IPanel
{
private:
    EffectEditPanel();
    virtual ~EffectEditPanel() = default;

public:
    static EffectEditPanel* Create();
    HRESULT Initialize() override;

    void Draw(GUIState& state) override;
    void Free()override;

};

NS_END
#pragma once
#include "Base.h"

#ifdef USE_IMGUI

NS_BEGIN(Engine)

class ImGuiManager :
    public Base
{
private:
    ImGuiManager();
    virtual ~ImGuiManager() = default;

public:
    static ImGuiManager* Create();
    HRESULT Initialize();

    void Free()override;

private:

};

NS_END

#endif
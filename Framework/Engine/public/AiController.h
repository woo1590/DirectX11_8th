#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL AiController :
    public Component
{
private:
    AiController(Object* owner);
    virtual ~AiController() = default;

public:
    static AiController* Create(Object* owner);
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END
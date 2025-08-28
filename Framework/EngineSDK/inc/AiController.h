#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL AiController :
    public Component
{
private:
    AiController(Object* owner);
    AiController(const AiController& prototype);
    virtual ~AiController() = default;

public:
    static AiController* Create(Object* owner);
    HRESULT Initialize_Prototype()override;
    HRESULT Initialize(InitDESC* arg)override;
    void Free()override;

    Component* Clone()override { return new AiController(*this); }

private:

};

NS_END
#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class InputSystem :
    public Base
{
private:
    InputSystem();
    virtual ~InputSystem();

public:
    static InputSystem* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END
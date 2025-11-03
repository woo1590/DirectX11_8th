#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL ICommand :
    public Base
{
protected:
    ICommand() = default;
    virtual ~ICommand() = default;

public:
    virtual void Execute() = 0;

protected:
    void Free()override;
};
NS_END
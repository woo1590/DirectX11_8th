#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Mesh :
    public Base
{
private:
    Mesh();
    virtual ~Mesh() = default;

public:
    static Mesh* Create();
    HRESULT Initialize();
    void Free()override;

private:

};

NS_END
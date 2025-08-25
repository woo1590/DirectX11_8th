#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Model :
    public Base
{
private:
    Model();
    virtual ~Model() = default;

public:
    static Model* Create();
    HRESULT Initialize();

    void Free()override;
private:

};

NS_END
#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL Shader :
    public Base
{
private:
    Shader();
    Shader(const Shader& rhg);
    virtual ~Shader() = default;

public:
    static Shader* Create();
    HRESULT Initialize();

    void Free()override;

private:

};

NS_END
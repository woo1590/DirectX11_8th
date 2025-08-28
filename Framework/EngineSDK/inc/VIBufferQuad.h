#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBufferQuad :
    public VIBuffer
{
private:
    VIBufferQuad();
    virtual ~VIBufferQuad() = default;

public:
    static VIBufferQuad* Create();
    HRESULT Initialize()override;

    void Free()override;

private:

};

NS_END
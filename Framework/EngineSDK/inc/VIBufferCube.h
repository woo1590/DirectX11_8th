#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBufferCube :
    public VIBuffer
{
private:
    VIBufferCube();
    virtual ~VIBufferCube() = default;

public:
    static VIBufferCube* Create();
    HRESULT Initialize()override;

    void Free()override;

private:

};

NS_END
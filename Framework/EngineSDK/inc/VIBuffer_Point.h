#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Point :
    public VIBuffer
{
private:
    VIBuffer_Point();
    virtual ~VIBuffer_Point() = default;

public:
    static VIBuffer_Point* Create();
    HRESULT Initialize()override;

    HRESULT BindBuffers()override;
    HRESULT Draw()override;

    void Free()override;

private:

};

NS_END
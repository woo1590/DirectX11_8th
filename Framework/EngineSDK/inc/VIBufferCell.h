#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBufferCell :
    public VIBuffer
{
private:
    VIBufferCell();
    virtual ~VIBufferCell() = default;

public:
    static VIBufferCell* Create(const _float3* points);
    HRESULT Initialize(const _float3* points);

    void Free()override;

private:

};

NS_END
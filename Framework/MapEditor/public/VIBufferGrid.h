#pragma once
#include "VIBuffer.h"

NS_BEGIN(MapEditor)

class VIBufferGrid :
    public VIBuffer
{
private:
    VIBufferGrid();
    virtual ~VIBufferGrid() = default;

public:
    static VIBufferGrid* Create();
    HRESULT Initialize()override;

private:

};

NS_END
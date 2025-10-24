#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Instancing abstract:
    public VIBuffer
{
protected:
    VIBuffer_Instancing();
    virtual ~VIBuffer_Instancing() = default;

public:

protected:
    ID3D11Buffer* m_VBInstancing = nullptr;
    _uint m_iNumInstances{};
};

NS_END
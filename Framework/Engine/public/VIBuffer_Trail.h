#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL VIBuffer_Trail :
    public VIBuffer
{
private:
    VIBuffer_Trail();
    virtual ~VIBuffer_Trail() = default;

public:
    static VIBuffer_Trail* Create(_uint numMaxPoints);
    HRESULT Initialize(_uint numMaxPoints);

    void UpdateBuffer(const VTX_TRAIL* vertices, _uint numPoints);

    HRESULT BindBuffers()override;
    HRESULT Draw()override;
    void Free()override;

private:

};

NS_END
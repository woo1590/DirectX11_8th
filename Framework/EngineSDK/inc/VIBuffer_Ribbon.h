#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class VIBuffer_Ribbon :
    public VIBuffer
{
private:
    VIBuffer_Ribbon();
    virtual ~VIBuffer_Ribbon() = default;

public:
    static VIBuffer_Ribbon* Create(_uint numMaxPoints);
    HRESULT Initialize(_uint numMaxPoints);

    void Free()override;

private:

};

NS_END
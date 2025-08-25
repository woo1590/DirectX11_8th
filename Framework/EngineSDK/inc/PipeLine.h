#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class PipeLine :
    public Base
{
private:
    PipeLine();
    virtual ~PipeLine() = default;

public:
    static PipeLine* Create();
    HRESULT Initialize();
    
    void XM_CALLCONV SetViewMatrix(_fmatrix viewMatrix);
    void XM_CALLCONV SetProjMatrix(_fmatrix projMatrix);
    
    _matrix XM_CALLCONV GetViewMatirx()const;
    _matrix XM_CALLCONV GetViewMatrixInverse()const;
    _matrix XM_CALLCONV GetProjMatrix()const;
    _matrix XM_CALLCONV GetProjMatrixInverse()const;

    void Free()override;

private:
    //보통 셰이더 바인딩 할 때 필요한것들
    _float4x4 m_ViewMatrix{};
    _float4x4 m_ViewMatrixInverse{};

    _float4x4 m_ProjMatrix{};
    _float4x4 m_ProjMatrixInverse{};

    _float3 m_CamPosition{};   

};

NS_END
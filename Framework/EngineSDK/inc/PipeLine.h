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
    void Update();

    void SetViewMatrix(_float4x4 viewMatrix) { m_ViewMatrix = viewMatrix; }
    void SetProjMatrix(_float4x4 projMatrix) { m_ProjMatrix = projMatrix; }
    
    // 히힛 오줌발싸
    _float4x4 GetViewMatirx()const { return m_ViewMatrix; }
    _float4x4 GetViewMatrixInverse()const { return m_ViewMatrixInverse; }
    _float4x4 GetProjMatrix()const { return m_ProjMatrix; }
    _float4x4 GetProjMatrixInverse()const { return m_ProjMatrixInverse; }

    _float3 GetCamPosition()const { return m_CamPosition; }

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
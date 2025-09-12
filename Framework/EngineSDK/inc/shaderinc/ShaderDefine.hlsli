#pragma pack_matrix(row_major)

cbuffer PerFrame : register(b0)
{
    float4x4 g_ViewMatrix;
    float4x4 g_ViewMatrixInverse;
    float4x4 g_ProjMatrix;
    float4x4 g_ProjMatrixInverse;
    float4 g_CamPosition;
    
    float4 g_LightColor;
    float4 g_LightDirection;
    float4 g_LightPosition;
    float g_LightRange;
    float3 pad;
}

cbuffer PerLightViewProj : register(b1)
{
    float4x4 g_LightViewMatrix;
    float4x4 g_LightProjMatrix;
}

cbuffer PerObject : register(b2)
{
    float4x4 g_WorldMatrix;
    float4x4 g_WorldMatrixInverse;
}
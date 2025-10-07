#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class ENGINE_DLL MaterialInstance :
    public Base
{
private:
    MaterialInstance();
    virtual ~MaterialInstance() = default;

public:
    static MaterialInstance* Create();
    HRESULT Initialize();
    HRESULT BindMaterialInstance(Shader* shader);

    void SetInt(_string name, _int param);
    void SetFloat(_string name, _float param);
    void SetFloat2(_string name, _float2 param);
    void SetFloat3(_string name, _float3 param);
    void SetFloat4(_string name, _float4 param);
    void SetFloat4x4(_string name, _float4x4 param);
    void Free()override;

private:
    std::unordered_map<_string, _int> m_IntParams;
    std::unordered_map<_string, _float> m_FloatParams;
    std::unordered_map<_string, _float2> m_Float2Params;
    std::unordered_map<_string, _float3> m_Float3Params;
    std::unordered_map<_string, _float4> m_Float4Params;
    std::unordered_map<_string, _float4x4> m_Float4x4Params;
};

NS_END
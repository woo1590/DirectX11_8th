#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class ENGINE_DLL Material :
    public Base
{
private:
    Material(Shader* pShader);
    virtual ~Material() = default;

public:
    static Material* Create(Shader* pShader);
    HRESULT Initialize();

    HRESULT BindMaterial(_uint passIndex);
    void SetParam(const _string& key, _int value) { m_IntParams[key] = value; }
    void SetParam(const _string& key, _float value) { m_FloatParams[key] = value; }
    void SetParam(const _string& key, _float2 value) { m_Float2Params[key] = value; }
    void SetParam(const _string& key, _float3 value) { m_Float3Params[key] = value; }
    void SetParam(const _string& key, _float4 value) { m_Float4Params[key] = value; }
    void SetParam(const _string& key, _float4x4 value) { m_Float4x4Params[key] = value; }
    void SetParam(const _string& key, ID3D11ShaderResourceView* value) { m_TexParams[key] = value; }

    void Free()override;

private:
    Shader* m_pShader = nullptr;

    std::unordered_map<_string, _int> m_IntParams;
    std::unordered_map<_string, _float> m_FloatParams;
    std::unordered_map<_string, _float2> m_Float2Params;
    std::unordered_map<_string, _float3> m_Float3Params;
    std::unordered_map<_string, _float4> m_Float4Params;
    std::unordered_map<_string, _float4x4> m_Float4x4Params;
    std::unordered_map<_string, ID3D11ShaderResourceView*> m_TexParams;

};

NS_END
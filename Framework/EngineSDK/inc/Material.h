#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Material :
    public Base
{
private:
    Material();
    virtual ~Material() = default;

public:
    static Material* Create();
    HRESULT Initialize();
    void Free()override;

    void SetParam(const std::string& key, _int value);
    void SetParam(const std::string& key, _float value);
    void SetParam(const std::string& key, _float2 value);
    void SetParam(const std::string& key, _float3 value);
    void SetParam(const std::string& key, _float4 value);

private:
    std::unordered_map<std::string, _int> m_IntParams;
    std::unordered_map<std::string, _float> m_FloatParams;
    std::unordered_map<std::string, _float2> m_Float2Params;
    std::unordered_map<std::string, _float3> m_Float3Params;
    std::unordered_map<std::string, _float4> m_Float4Params;

};

NS_END
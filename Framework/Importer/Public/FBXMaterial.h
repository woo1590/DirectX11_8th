#pragma once
#include "Material.h"

NS_BEGIN(Engine)
class Shader;
NS_END

NS_BEGIN(Importer)

class FBXMaterial :
    public Material
{
private:
    FBXMaterial();
    virtual ~FBXMaterial() = default;

public:
    static FBXMaterial* Create(Shader* pShader, const _string& shaderTag, aiMaterial* aiMat, const _string& modelFilePath);
    HRESULT Initialize(Shader* pShader, const _string& shaderTag, aiMaterial* aiMat, const _string& modelFilePath);

    HRESULT ExportMaterialFormat(MTRL_FORMAT& mtrlFormat);

    void Free()override;
private:
    std::unordered_map<_string, std::vector<_string>> m_TexNames;

    _string m_strShaderTag{};
};

NS_END
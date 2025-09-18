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
    HRESULT ConvertToDDS(const _string& outFilePath);
    _string GetName()const { return m_strName; }
    void Free()override;

#ifdef USE_IMGUI
    void RenderInspector();
    void DrawTextureGrid(const _string& tag);
#endif

private:
    HRESULT AddTexture(const _string& slotTag, const _string& filePath);

    _string m_strName;
    _string m_strModelFilePath{};
    _string m_strShaderTag{};
    std::unordered_map<_string, std::vector<_string>> m_TexNames;
};

NS_END
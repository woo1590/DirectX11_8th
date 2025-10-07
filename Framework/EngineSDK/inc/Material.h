#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class Texture;
class MaterialInstance;
class ENGINE_DLL Material :
    public Base
{
protected:
    Material();
    virtual ~Material() = default;

public:
    static Material* Create(Shader* pShader);
    static Material* Create(MTRL_FORMAT mtrlFormat, const _string& modelFilePath);
    HRESULT Initialize(Shader* pShader);
    HRESULT Initialize(MTRL_FORMAT mtrlFormat, const _string& modelFilePath);

    HRESULT BindMaterial(const _string& passTag, _int frameIndex, MaterialInstance* mtrlInstance);
    void SetTexture(const _string& key, Texture* value);

    void Free()override;

protected:
    Shader* m_pShader = nullptr;

    std::unordered_map<_string, std::vector<Texture*>> m_TexParams;

};

NS_END
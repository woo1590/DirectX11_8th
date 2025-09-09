#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Shader;
class Texture;
class ENGINE_DLL Material :
    public Base
{
private:
    Material(Shader* pShader);
    virtual ~Material() = default;

public:
    static Material* Create(Shader* pShader);
    HRESULT Initialize();

    HRESULT BindMaterial(const _string& passTag, _int frameIndex);
    void SetTexture(const _string& key, Texture* value);

    void Free()override;

private:
    Shader* m_pShader = nullptr;

    std::unordered_map<_string, Texture*> m_TexParams;

};

NS_END
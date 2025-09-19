#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Texture;
class VIBuffer;
class Shader;
class Material;
class Model;
class AnimationClip;
class ResourceManager :
    public Base
{
private:
    ResourceManager();
    virtual ~ResourceManager() = default;

public:
    static ResourceManager* Create(_uint numLevel);
    HRESULT Initialize(_uint numLevel);

    HRESULT LoadBuffer(_uint levelID, const _string& key, VIBuffer* pBuffer);
    HRESULT LoadBufferFromModel() { return S_OK; }
    HRESULT LoadMaterialFromJson(_uint levelID, const _string& filePath, const _string& key);
    HRESULT LoadShaderFromFile(const _string& filePath, const _string& key,
                               const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);
    HRESULT LoadModelFromFile(_uint levelID, const _string& filePath, const _string& key);
    HRESULT LoadAnimationSetFromFile(_uint levelID, const _string& filePath, const _string& key);

    VIBuffer*   GetBuffer(_uint levelID, const _string& key);
    Shader*     GetShader(const _string& key);
    Material*   GetMaterial(_uint levelID, const _string& key);
    Model*      GetModel(_uint levelID, const _string& key);
    ANIMATION_SET GetAnimation(_uint levelID, const _string& key);

    void Clear(_uint levelID);

    void Free()override;

private:
    HRESULT LoadTextureFromFile(_uint levelID, const _string& filePath);
    Texture* GetTexture(_uint levelID, const _string& key);

    _uint m_iNumLevel = 0;

    std::vector<std::unordered_map<_string, VIBuffer*>> m_Buffers;
    std::vector<std::unordered_map<_string, Material*>> m_Materials;
    std::unordered_map<_string, Shader*> m_Shaders;
    std::vector<std::unordered_map<_string, Texture*>> m_Textures;
    std::vector<std::unordered_map<_string, Model*>> m_Models;
    std::vector<std::unordered_map<_string, ANIMATION_SET>> m_AnimationSets;
};

NS_END
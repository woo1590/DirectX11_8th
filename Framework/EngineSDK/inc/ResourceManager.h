#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class VIBuffer;
class Shader;
class Material;
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
    HRESULT LoadShader(_uint levelID, const _wstring& filePath, const _string& key, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);

    VIBuffer* GetBuffer(_uint levelID, const _string& key);
    Shader* GetShader(_uint levelID, const _string& key);

    void Clear(_uint levelID);
    void Free()override;

private:
    _uint m_iNumLevel = 0;

    std::vector<std::unordered_map<_string, VIBuffer*>> m_Buffers;
    std::vector<std::unordered_map<_string, Material*>> m_Materials;
    std::vector<std::unordered_map<_string, Shader*>> m_Shaders;
};

NS_END
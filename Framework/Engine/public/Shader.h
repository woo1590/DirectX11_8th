#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class Texture;
class ENGINE_DLL Shader final:
    public Base
{
private:
    Shader();
    Shader(const Shader& prototype);
    virtual ~Shader() = default;

public:
    static Shader* Create(const _string& filePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);
    HRESULT Initialize(const _string& filePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement);

    HRESULT Apply(const _string& passTag);
    HRESULT BindTextureValue(const _string& name, Texture* value);
    HRESULT SetConstantBuffer(ID3D11Buffer* constantBuffer, const _string& name);

    void Free()override;

private:
    _uint m_iNumPasses = 0;
    ID3DX11Effect* m_pEffect = nullptr; 
    std::unordered_map<_string, ID3D11InputLayout*> m_InputLayouts;
    std::unordered_map<_string, ID3DX11EffectPass*> m_EffectPasses;

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
};

NS_END
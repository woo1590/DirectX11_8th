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
    HRESULT BindMatrix(const _string& name, const _float4x4* value);
    HRESULT BindRawValue(const _string& name, const void* value, _uint size);
    HRESULT BindTextureValue(const _string& name, Texture* value);
    HRESULT SetConstantBuffer(const _string& name, ID3D11Buffer* constantBuffer);

    void Free()override;

private:
    HRESULT BuildMetaData();

    _uint m_iNumPasses = 0;
    ID3DX11Effect* m_pEffect = nullptr; 
    std::unordered_map<_string, ID3D11InputLayout*> m_InputLayouts;
    std::unordered_map<_string, ID3DX11EffectPass*> m_EffectPasses;

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    std::unordered_map<_string, CBUFFER_META> m_CBuffers;
    std::unordered_map<_string, GLOBAL_VARIABLE_META> m_GlobalVariables;
    std::unordered_map<_string, RESOURCE_META> m_Resources;
};

NS_END
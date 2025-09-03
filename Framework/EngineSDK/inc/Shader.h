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

    HRESULT Apply(_uint passIndex);
    HRESULT BindTextureValue(const _string& name, Texture* value, _int frameIndex);

    void Free()override;

private:
    _uint m_iNumPasses = 0;
    ID3DX11Effect* m_pEffect = nullptr; 
    std::vector<ID3D11InputLayout*> m_InputLayouts;

    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;
};

NS_END
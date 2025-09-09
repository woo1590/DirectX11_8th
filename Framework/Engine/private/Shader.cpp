#include "EnginePCH.h"
#include "Shader.h"
#include "EngineCore.h"
#include "Renderer.h"
#include "Texture.h"

Shader::Shader():
    m_pDevice(EngineCore::GetInstance()->GetDevice()),
    m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
    m_pDevice->AddRef();
    m_pDeviceContext->AddRef();
}

Shader::Shader(const Shader& prototype)
{
}

Shader* Shader::Create(const _string& filePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
    Shader* Instance = new Shader();

    if (FAILED(Instance->Initialize(filePath,pElement,numElement)))
        Safe_Release(Instance);

    return Instance;
}

HRESULT Shader::Initialize(const _string& filePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint numElement)
{
    std::filesystem::path p = std::filesystem::u8path(filePath);
    _wstring path = p.wstring();

    _uint shaderFlag = 0;

#ifdef _DEBUG
    shaderFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    shaderFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

    if (FAILED(D3DX11CompileEffectFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, shaderFlag, 0, m_pDevice, &m_pEffect, nullptr)))
        return E_FAIL;

    /* 셰이더 안에서 정의된 패스들이 내가 그리고자 하는 정점 구조체와 호환이 되는지? */
    ID3DX11EffectTechnique* technique = nullptr;
    technique = m_pEffect->GetTechniqueByIndex(0);
    if (!technique)
        return E_FAIL;

    D3DX11_TECHNIQUE_DESC techniqueDesc{};
    technique->GetDesc(&techniqueDesc);

    m_iNumPasses = techniqueDesc.Passes;
    m_InputLayouts.reserve(m_iNumPasses);

    for (_int i = 0; i < m_iNumPasses; ++i)
    {
        ID3D11InputLayout* inputLayout = nullptr;
        ID3DX11EffectPass* pass = technique->GetPassByIndex(static_cast<uint32_t>(i));

        D3DX11_PASS_DESC passDesc{};
        pass->GetDesc(&passDesc);

        if (FAILED(m_pDevice->CreateInputLayout(pElement, numElement, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &inputLayout)))
            return E_FAIL;

        _string passName = passDesc.Name;

        m_InputLayouts[passName] = inputLayout;
        m_EffectPasses[passName] = pass;
    }

    /* Renderer에서 들고 있는 전역적인 상수버퍼와 현재 effect객체 연결 -> perframe, perlight, perobject..*/
    EngineCore::GetInstance()->GetRenderer()->ConnectConstantBuffer(m_pEffect);

    return S_OK;
}

HRESULT Shader::Apply(const _string& passTag)
{
    m_pDeviceContext->IASetInputLayout(m_InputLayouts[passTag]);

    auto iter = m_EffectPasses.find(passTag);
    if (iter == m_EffectPasses.end())
    {
        MSG_BOX("Apply Failed : Pass not exist");
        return E_FAIL;
    }

    return iter->second->Apply(0, m_pDeviceContext);
}

HRESULT Shader::BindTextureValue(const _string& name, Texture* value, _int frameIndex)
{
    ID3D11ShaderResourceView* srv = value->GetSRV(frameIndex);
    if (!srv)
        return E_FAIL;

    ID3DX11EffectVariable* variable = m_pEffect->GetVariableByName(name.c_str());
    if (!variable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* srvVariable = variable->AsShaderResource();
    if (!srvVariable)
        return E_FAIL;
    
    return srvVariable->SetResource(srv);
}

void Shader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);

    for (auto& pair : m_InputLayouts)
        Safe_Release(pair.second);
    m_InputLayouts.clear();

    m_EffectPasses.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pDeviceContext);
}

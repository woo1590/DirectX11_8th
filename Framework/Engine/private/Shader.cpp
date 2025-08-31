#include "EnginePCH.h"
#include "Shader.h"
#include "EngineCore.h"
#include "Renderer.h"

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

        m_InputLayouts.push_back(inputLayout);
    }

    EngineCore::GetInstance()->GetRenderer()->ConnectConstantBuffer(m_pEffect);

    return S_OK;
}

HRESULT Shader::Apply(_uint passIndex)
{
    m_pDeviceContext->IASetInputLayout(m_InputLayouts[passIndex]);

    ID3DX11EffectPass* pass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(passIndex);
    if (!pass)
        return E_FAIL;

    return pass->Apply(0, m_pDeviceContext);
}

HRESULT Shader::SetValue(const _string& name, _float4x4 value)
{
    ID3DX11EffectVariable* variable = m_pEffect->GetVariableByName(name.c_str());
    if (!variable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* matrixVariable = variable->AsMatrix();
    if (!matrixVariable)
        return E_FAIL;

    return matrixVariable->SetMatrix(reinterpret_cast<const _float*>(&value));
}

HRESULT Shader::SetValue(const _string& name, ID3D11ShaderResourceView* value)
{
    ID3DX11EffectVariable* variable = m_pEffect->GetVariableByName(name.c_str());
    if (!variable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* srvVariable = variable->AsShaderResource();
    if (!srvVariable)
        return E_FAIL;

    return srvVariable->SetResource(value);
}

void Shader::Free()
{
    __super::Free();

    Safe_Release(m_pEffect);
    for (auto& layout : m_InputLayouts)
        Safe_Release(layout);

    Safe_Release(m_pDevice);
    Safe_Release(m_pDeviceContext);
}

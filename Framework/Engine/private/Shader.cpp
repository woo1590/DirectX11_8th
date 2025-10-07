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

    ID3DBlob* error = nullptr;

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

        _string passName = passDesc.Name;

        m_InputLayouts[passName] = inputLayout;
        m_EffectPasses[passName] = pass;
    }

    if (FAILED(BuildMetaData()))
        return E_FAIL;

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

HRESULT Shader::BindMatrix(const _string& name, const _float4x4* value)
{
    auto iter = m_GlobalVariables.find(name);
    if (iter == m_GlobalVariables.end())
        return E_FAIL;

    return iter->second.variableHandle->AsMatrix()->BindMatrixValue(reinterpret_cast<const _float*>(value));
}

HRESULT Shader::BindRawValue(const _string& name, const void* value, _uint size)
{
    auto iter = m_GlobalVariables.find(name);
    if (iter == m_GlobalVariables.end())
        return E_FAIL;

    return iter->second.variableHandle->SetRawValue(value, 0, size);
}

HRESULT Shader::BindTextureValue(const _string& name, Texture* value)
{
    auto iter = m_Resources.find(name);
    if (iter == m_Resources.end())
        return E_FAIL;

    auto srv = value->GetSRV();

    return iter->second.srvHandle->SetResource(srv);
}

HRESULT Shader::SetConstantBuffer(const _string& name, ID3D11Buffer* constantBuffer)
{
    auto iter = m_CBuffers.find(name);
    if (iter == m_CBuffers.end())
        return E_FAIL;

    return iter->second.bufferHandle->SetConstantBuffer(constantBuffer);
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

HRESULT Shader::BuildMetaData()
{
    D3DX11_EFFECT_DESC effectDesc;
    m_pEffect->GetDesc(&effectDesc);

    for (_uint i = 0; i < effectDesc.ConstantBuffers; ++i)
    {
        ID3DX11EffectConstantBuffer* cbuffer = m_pEffect->GetConstantBufferByIndex(i);
        if (cbuffer && cbuffer->IsValid())
        {
            D3DX11_EFFECT_VARIABLE_DESC cbufferDesc{};
            D3DX11_EFFECT_TYPE_DESC cbufferTypeDesc{};
            cbuffer->GetDesc(&cbufferDesc);
            cbuffer->GetType()->GetDesc(&cbufferTypeDesc);

            //global variable
            if (!strcmp(cbufferDesc.Name, "$Globals"))
            {
                for (_uint j = 0; j < cbufferTypeDesc.Members; ++j)
                {
                    ID3DX11EffectVariable* member = cbuffer->GetMemberByIndex(j);

                    D3DX11_EFFECT_VARIABLE_DESC varDesc{};
                    D3DX11_EFFECT_TYPE_DESC varTypeDesc{};
                    member->GetDesc(&varDesc);
                    member->GetType()->GetDesc(&varTypeDesc);

                    GLOBAL_VARIABLE_META varMeta{};
                    varMeta.name = varDesc.Name;
                    varMeta.varType = varTypeDesc.TypeName;
                    varMeta.size = varTypeDesc.UnpackedSize;
                    varMeta.offset = varDesc.BufferOffset;
                    varMeta.rows = varTypeDesc.Rows;
                    varMeta.cols = varTypeDesc.Columns;
                    varMeta.elements = varTypeDesc.Elements;
                    varMeta.variableHandle = member;

                    m_GlobalVariables.emplace(varMeta.name, varMeta);
                }
            }
            /*cbuffer*/
            else
            {
                CBUFFER_META cbufferMeta{};
                cbufferMeta.name = cbufferDesc.Name;
                cbufferMeta.size = cbufferTypeDesc.UnpackedSize;
                cbufferMeta.bufferHandle = cbuffer;
           
                for (_uint j = 0; j < cbufferTypeDesc.Members; ++j)
                {
                    ID3DX11EffectVariable* member = cbuffer->GetMemberByIndex(j);

                    D3DX11_EFFECT_VARIABLE_DESC varDesc{};
                    D3DX11_EFFECT_TYPE_DESC varTypeDesc{};
                    member->GetDesc(&varDesc);
                    member->GetType()->GetDesc(&varTypeDesc);

                    VARIABLE_META varMeta{};
                    varMeta.name = varDesc.Name;
                    varMeta.varType = varTypeDesc.TypeName;
                    varMeta.size = varTypeDesc.UnpackedSize;
                    varMeta.offset = varDesc.BufferOffset;
                    varMeta.rows = varTypeDesc.Rows;
                    varMeta.cols = varTypeDesc.Columns;
                    varMeta.elements = varTypeDesc.Elements;

                    cbufferMeta.variables.push_back(varMeta);
                }

                m_CBuffers.emplace(cbufferMeta.name, cbufferMeta);
            }
        }
    }

    for (_uint i = 0; i < effectDesc.GlobalVariables; ++i)
    {
        ID3DX11EffectShaderResourceVariable* srvVar = m_pEffect->GetVariableByIndex(i)->AsShaderResource();

        if (srvVar && srvVar->IsValid())
        {
            D3DX11_EFFECT_VARIABLE_DESC varDesc{};
            D3DX11_EFFECT_TYPE_DESC varTypeDesc{};
            srvVar->GetDesc(&varDesc);
            srvVar->GetType()->GetDesc(&varTypeDesc);

            RESOURCE_META resMeta{};
            resMeta.name = varDesc.Name;
            resMeta.typeName = varTypeDesc.TypeName;
            resMeta.srvHandle = srvVar;

            m_Resources.emplace(resMeta.name, resMeta);
        }
    }

    return S_OK;
}

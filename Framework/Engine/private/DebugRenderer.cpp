#include "EnginePCH.h"
#include "DebugRenderer.h"
#include "EngineCore.h"
#include "Shader.h"
#include "VIBuffer.h"


DebugRenderer::DebugRenderer()
{
}

DebugRenderer* DebugRenderer::Create()
{
	DebugRenderer* Instance = new DebugRenderer();

	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT DebugRenderer::Initialize()
{
	auto engine = EngineCore::GetInstance();

	m_pDevice = engine->GetDevice();
	m_pDeviceContext = engine->GetDeviceContext();

	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();

	m_pDebugShader = Shader::Create("../bin/shaderfiles/Shader_Debug.hlsl", VTXPOS::elements, VTXPOS::numElement);
	if (!m_pDebugShader)
	{
		MSG_BOX("Failed to create : Debug Shader");
		return E_FAIL;
	}

	/*Create perframe buffer*/
	D3D11_BUFFER_DESC perFrameDesc{};
	perFrameDesc.ByteWidth = sizeof(DEBUG_CBPerFrame);
	perFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	perFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perFrameDesc.MiscFlags = 0;
	perFrameDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&perFrameDesc, nullptr, &m_pCBPerFrame)))
		return E_FAIL;

	/*Create perdraw buffer*/
	D3D11_BUFFER_DESC perDrawDesc{};
	perDrawDesc.ByteWidth = sizeof(DEBUG_CBPerDraw);
	perDrawDesc.Usage = D3D11_USAGE_DYNAMIC;
	perDrawDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	perDrawDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	perDrawDesc.MiscFlags = 0;
	perDrawDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&perDrawDesc, nullptr, &m_pCBPerDraw)))
		return E_FAIL;

	/*Connect constant buffer to debug shader*/
	if (FAILED(m_pDebugShader->SetConstantBuffer("DebugPerFrame", m_pCBPerFrame)))
		return E_FAIL;
	if (FAILED(m_pDebugShader->SetConstantBuffer("DebugPerDraw", m_pCBPerDraw)))
		return E_FAIL;

	return S_OK;
}

HRESULT DebugRenderer::BeginFrame()
{
	auto engine = EngineCore::GetInstance();

	D3D11_MAPPED_SUBRESOURCE mapData{};
	DEBUG_CBPerFrame cbPerFrame{};
	CAMERA_CONTEXT camContext = engine->GetCameraContext();
	cbPerFrame.viewMatrix = camContext.viewMatrix;
	cbPerFrame.projMatrix = camContext.projMatrix;

	m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapData);
	memcpy_s(mapData.pData, sizeof(DEBUG_CBPerFrame), &cbPerFrame, sizeof(DEBUG_CBPerFrame));
	m_pDeviceContext->Unmap(m_pCBPerFrame, 0);

	return S_OK;
}

HRESULT DebugRenderer::RenderNavMeshDebug(const std::vector<RenderProxy>& proxies)
{
	for (const auto& proxy : proxies) 
		DrawDebugProxy(proxy, "NavMeshDebug_Pass");

	return S_OK;
}

void DebugRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pCBPerFrame);
	Safe_Release(m_pCBPerDraw);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDebugShader);
}

HRESULT DebugRenderer::DrawDebugProxy(const RenderProxy& proxy, const _string& passTag)
{
	D3D11_MAPPED_SUBRESOURCE mapData{};
	DEBUG_CBPerDraw cbPerDraw{};
	cbPerDraw.worldMatrix = proxy.worldMatrix;

	m_pDeviceContext->Map(m_pCBPerDraw, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapData);
	memcpy_s(mapData.pData, sizeof(DEBUG_CBPerDraw), &cbPerDraw, sizeof(DEBUG_CBPerDraw));
	m_pDeviceContext->Unmap(m_pCBPerDraw, 0);

	if (FAILED(proxy.buffer->BindBuffers()))
		return E_FAIL;

	if (FAILED(m_pDebugShader->Apply(passTag)))
		return E_FAIL;

	return proxy.buffer->Draw();
}

#include "EnginePCH.h"
#include "Renderer.h"
#include "EngineCore.h"
#include "Material.h"
#include "LightProxy.h"
#include "Shader.h"
#include "VIBufferQuad.h"

Renderer::Renderer()
	:m_pDevice(EngineCore::GetInstance()->GetDevice()),
	m_pDeviceContext(EngineCore::GetInstance()->GetDeviceContext())
{
	m_pDevice->AddRef();
	m_pDeviceContext->AddRef();
}

Renderer* Renderer::Create()
{
	Renderer* Instance = new Renderer();
	
	if (FAILED(Instance->Initialize()))
		Safe_Release(Instance);

	return Instance;
}

HRESULT Renderer::Initialize()
{
	/*----Constant Buffer Per Frame----*/
	D3D11_BUFFER_DESC cbPerFrameDesc{};
	cbPerFrameDesc.ByteWidth = sizeof(CBPerFrame);
	cbPerFrameDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerFrameDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerFrameDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerFrameDesc.MiscFlags = 0;
	cbPerFrameDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerFrameDesc, nullptr, &m_pCBPerFrame)))
		return E_FAIL;

	/*----Constant Buffer Per Light----*/
	D3D11_BUFFER_DESC cbPerLightDesc{};
	cbPerLightDesc.ByteWidth = sizeof(CBPerLight);
	cbPerLightDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerLightDesc.MiscFlags = 0;
	cbPerLightDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerLightDesc, nullptr, &m_pCBPerLight)))
		return E_FAIL;

	/*----Constant Buffer Per Object----*/
	D3D11_BUFFER_DESC cbPerObjectDesc{};
	cbPerObjectDesc.ByteWidth = sizeof(CBPerObject);
	cbPerObjectDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbPerObjectDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbPerObjectDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbPerObjectDesc.MiscFlags = 0;
	cbPerObjectDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbPerObjectDesc, nullptr, &m_pCBPerObject)))
		return E_FAIL;

	/*----Constant Buffer For Bones----*/
	D3D11_BUFFER_DESC cbBones{};
	cbBones.ByteWidth = sizeof(_float4x4) * MAX_BONES;
	cbBones.Usage = D3D11_USAGE_DYNAMIC;
	cbBones.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbBones.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbBones.MiscFlags = 0;
	cbBones.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&cbBones, nullptr, &m_pCBBonePalatte)))
		return E_FAIL;

	/*----deferred rendering----*/
	_uint numViewPort = 1;
	D3D11_VIEWPORT viewPort{};
	m_pDeviceContext->RSGetViewports(&numViewPort, &viewPort);

	m_pBuffer = VIBufferQuad::Create();
	m_pShader = Shader::Create("../bin/shaderfiles/Shader_Deferred.hlsl", VTXTEX::elements, VTXTEX::numElement);
	ConnectConstantBuffer(m_pShader);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(viewPort.Width, viewPort.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(viewPort.Width, viewPort.Height, 0.f, 1.f));

	if (FAILED(Initialize_DeferredTargets(viewPort)))
		return E_FAIL;

	return S_OK;
}

HRESULT Renderer::BeginFrame()
{
	/* 이번 프레임을 그릴 때 사용할 카메라 view, proj   라이트 view proj 세팅*/

	auto engine = EngineCore::GetInstance();

	{
		D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
		CBPerFrame perFrame{};

		/*View, Proj Matrix*/
		CAMERA_CONTEXT camContext = engine->GetCameraContext();
		perFrame.viewMatrix = camContext.viewMatrix;
		perFrame.projMatrix = camContext.projMatrix;
		_float3 campos = camContext.camPosition;
		perFrame.camPosition = _float4(campos.x, campos.y, campos.z, 1.f);
		perFrame.farZ = camContext.farZ;
		perFrame.nearZ = camContext.nearZ;

		m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
		memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
		m_pDeviceContext->Unmap(m_pCBPerFrame, 0);
	}

	return S_OK;
}

HRESULT Renderer::RenderPriority(const std::vector<RenderProxy>& proxies)
{
	for (const auto& proxy : proxies)
		DrawProxy(proxy);

	return S_OK;
}

HRESULT Renderer::RenderNonBlend(const std::vector<RenderProxy>& proxies)
{
	auto engine = EngineCore::GetInstance();
	engine->BeginMRT("MRT_Objects");

	for (const auto& proxy : proxies)
		DrawProxy(proxy);

	engine->EndMRT();

	return S_OK;
}

HRESULT Renderer::RenderLight(const std::vector<LightProxy>& proxies)
{
	auto engine = EngineCore::GetInstance();
	engine->BeginMRT("MRT_LightAcc");

	engine->BindShaderResource(m_pShader, "Target_Normal", "g_NormalTexture");
	engine->BindShaderResource(m_pShader, "Target_Depth", "g_DepthTexture");

	CAMERA_CONTEXT context = engine->GetCameraContext();
	D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
	CBPerFrame perFrame{};
	perFrame.viewMatrix = m_ViewMatrix;
	perFrame.projMatrix = m_ProjMatrix;
	perFrame.viewMatrixInverse = context.viewMatrixInverse;
	perFrame.projMatrixInverse = context.projMatrixInverse;
	perFrame.camPosition = _float4{ context.camPosition.x,context.camPosition.y,context.camPosition.z,1.f };
	perFrame.nearZ = context.nearZ;
	perFrame.farZ = context.farZ;

	D3D11_MAPPED_SUBRESOURCE perObjectData{};
	CBPerObject perObject{};
	perObject.worldMatrix = m_WorldMatrix;

	m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
	memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
	m_pDeviceContext->Unmap(m_pCBPerFrame, 0);

	m_pDeviceContext->Map(m_pCBPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &perObjectData);
	memcpy_s(perObjectData.pData, sizeof(CBPerObject), &perObject, sizeof(CBPerObject));
	m_pDeviceContext->Unmap(m_pCBPerObject, 0);

	for (const auto& proxy : proxies)
	{
		D3D11_MAPPED_SUBRESOURCE cbPerLightData{};
		CBPerLight perLight{};
		perLight.lightColor = proxy.lightColor;
		perLight.lightDirection = proxy.lightDirection;
		perLight.lightPosition = proxy.lightPosition;
		perLight.lightRange = proxy.lightRange;

		m_pDeviceContext->Map(m_pCBPerLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerLightData);
		memcpy_s(cbPerLightData.pData, sizeof(CBPerLight), &perLight, sizeof(CBPerLight));
		m_pDeviceContext->Unmap(m_pCBPerLight, 0);

		m_pBuffer->BindBuffers();

		if (proxy.type == LightType::Directional)
			m_pShader->Apply("Directional_Light");
		else
			m_pShader->Apply("Point_Light");

		m_pBuffer->Draw();
	}

	engine->EndMRT();

	return S_OK;
}

HRESULT Renderer::RenderCombined()
{
	auto engine = EngineCore::GetInstance();

	engine->BindShaderResource(m_pShader, "Target_Diffuse", "g_DiffuseTexture");
	engine->BindShaderResource(m_pShader, "Target_Shade", "g_ShadeTexture");
	engine->BindShaderResource(m_pShader, "Target_LightSpecular", "g_LightSpecularTexture");

	m_pBuffer->BindBuffers();
	m_pShader->Apply("Combined_Pass");
	m_pBuffer->Draw();

	return S_OK;
}

HRESULT Renderer::RenderBlend(const std::vector<RenderProxy>& proxies)
{
	auto engine = EngineCore::GetInstance();

	{
		D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
		CBPerFrame perFrame{};

		/*View, Proj Matrix*/
		CAMERA_CONTEXT camContext = engine->GetCameraContext();
		perFrame.viewMatrix = camContext.viewMatrix;
		perFrame.projMatrix = camContext.projMatrix;
		_float3 campos = camContext.camPosition;
		perFrame.camPosition = _float4(campos.x, campos.y, campos.z, 1.f);

		m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
		memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
		m_pDeviceContext->Unmap(m_pCBPerFrame, 0);
	}

	for (const auto& proxy : proxies)
		DrawProxy(proxy);

	return S_OK;
}

HRESULT Renderer::RenderUI(const std::vector<RenderProxy>& proxies)
{
	/* ui 전용 view proj생성 (임시용) */
	D3D11_MAPPED_SUBRESOURCE cbPerFrameData{};
	CBPerFrame perFrame{};
	perFrame.viewMatrix = m_ViewMatrix;
	perFrame.projMatrix = m_ProjMatrix;

	m_pDeviceContext->Map(m_pCBPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &cbPerFrameData);
	memcpy_s(cbPerFrameData.pData, sizeof(CBPerFrame), &perFrame, sizeof(CBPerFrame));
	m_pDeviceContext->Unmap(m_pCBPerFrame, 0);

	for (const auto& proxy : proxies)
		DrawProxy(proxy);

	return S_OK;
}

HRESULT Renderer::Initialize_DeferredTargets(D3D11_VIEWPORT viewPort)
{
	auto engine = EngineCore::GetInstance();

	/*add render targets*/
	{
		if (FAILED(engine->AddRenderTarget("Target_Diffuse", viewPort.Width, viewPort.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
		if (FAILED(engine->AddRenderTarget("Target_Normal", viewPort.Width, viewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
		if (FAILED(engine->AddRenderTarget("Target_Specular", viewPort.Width, viewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
		if (FAILED(engine->AddRenderTarget("Target_Depth", viewPort.Width, viewPort.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
		if (FAILED(engine->AddRenderTarget("Target_Shade", viewPort.Width, viewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
		if (FAILED(engine->AddRenderTarget("Target_LightSpecular", viewPort.Width, viewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
	}
	
	/*add mrt object*/
	{
		if (FAILED(engine->AddMRT("MRT_Objects", "Target_Diffuse")))
			return E_FAIL;
		if (FAILED(engine->AddMRT("MRT_Objects", "Target_Normal")))
			return E_FAIL;
		if (FAILED(engine->AddMRT("MRT_Objects", "Target_Specular")))
			return E_FAIL;
		if (FAILED(engine->AddMRT("MRT_Objects", "Target_Depth")))
			return E_FAIL;
	}
	/*add mrt light acc*/
	{
		if (FAILED(engine->AddMRT("MRT_LightAcc", "Target_Shade")))
			return E_FAIL;
		if (FAILED(engine->AddMRT("MRT_LightAcc", "Target_LightSpecular")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT Renderer::DrawProxy(const RenderProxy& proxy)
{
	{
		D3D11_MAPPED_SUBRESOURCE perObjectData{};
		CBPerObject perObject{};
		perObject.worldMatrix = proxy.worldMatrix;
		 
		m_pDeviceContext->Map(m_pCBPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &perObjectData);
		memcpy_s(perObjectData.pData, sizeof(CBPerObject), &perObject, sizeof(CBPerObject));
		m_pDeviceContext->Unmap(m_pCBPerObject, 0);

		if (proxy.numBones)
		{
			D3D11_MAPPED_SUBRESOURCE bonesData{};
			m_pDeviceContext->Map(m_pCBBonePalatte, 0, D3D11_MAP_WRITE_DISCARD, 0, &bonesData);
			memcpy_s(bonesData.pData, sizeof(_float4x4) * MAX_BONES, proxy.boneMatrices, sizeof(_float4x4) * proxy.numBones);
			m_pDeviceContext->Unmap(m_pCBBonePalatte, 0);
		}
	}

	if (FAILED(proxy.buffer->BindBuffers()))
		return E_FAIL;

	if (FAILED(proxy.material->BindMaterial(proxy.frameIndex, proxy.materialInstance)))
		return E_FAIL;

	return proxy.buffer->Draw();
}

HRESULT Renderer::ConnectConstantBuffer(Shader* shader)
{
	if (FAILED(shader->SetConstantBuffer("PerFrame", m_pCBPerFrame)))
		return E_FAIL;
	
	if (FAILED(shader->SetConstantBuffer("PerObject", m_pCBPerObject)))
		return E_FAIL;
	
	if (FAILED(shader->SetConstantBuffer("BoneMatrices", m_pCBBonePalatte)))
		return E_FAIL;

	if (FAILED(shader->SetConstantBuffer("PerLight", m_pCBPerLight)))
		return E_FAIL;

	return S_OK;
}

void Renderer::Free()
{
	__super::Free();
	Safe_Release(m_pBuffer);
	Safe_Release(m_pShader);

	Safe_Release(m_pCBPerFrame);
	Safe_Release(m_pCBPerLight);
	Safe_Release(m_pCBPerObject);
	Safe_Release(m_pCBBonePalatte);

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
